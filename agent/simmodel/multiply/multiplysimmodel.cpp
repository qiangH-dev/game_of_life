#include "multiplysimmodel.h"
#include <random>
#include <unordered_map>

#include <agent/agentprovider.h>
#include <gateway/gateway_constants.h>
#include <gbbinfra/descriptorregister.h>

#include <net/phoneyou/gamelife/bb/Descriptors.pb.h>
#include <net/phoneyou/gamelife/bb/Messages.pb.h>



MultiplySimModel::MultiplySimModel()
    : EntitySimModel("MULTIPLY")
{
}

void MultiplySimModel::initialize()
{
    using bb::message::DimensionsChanged;
    listenerMessage<DimensionsChanged>(
        [this](const DimensionsChanged& _msg) {
            updateDimensions(_msg.cell_x(), _msg.cell_y());
        });
    using bb::message::AroundCells;
    using bb::message::AroundCellsResponse;
    registerRPCService<AroundCells, AroundCellsResponse>(
        "around_cells",
        [this](const AroundCells& _req, AroundCellsResponse& _res) {
            bb::descriptor::Position _pos;
            if (amster::StatusID::SUCCESS != readData(_req.center_id(), _pos)) {
                _res.mutable_response()->set_code(bb::ResponseCode::Failed);
                return;
            }
            auto _center = covid19::GridPt{ _pos.x(), _pos.y() };
            // 园内接正方形
            absl::ReaderMutexLock _lock(&gridMutex_);
            for (int r = 1; r <= _req.radius(); r++) {
                auto _circle = _res.mutable_circle_cells()->Add();
                _circle->set_radius(1);
                // 内切圆面积 2ΠR
                auto _ids = _circle->mutable_ids();
                _ids->Reserve(std::round(8 * std::atan(1) * r));
                const auto _inscribed = static_cast<int>(std::floor(r / std::sqrt(2)));
                for (int i = -r; i <= r; i++)
                    for (int j = -r; j <= r; j++) {
                        bool _hint{ std::abs(i) <= _inscribed
                            && std::abs(i) > r - 1
                            && std::abs(j) > r - 1
                            && std::abs(j) <= _inscribed };
                        if (!_hint) {
                            auto _d = static_cast<int>(std::round(std::hypot(i, j)));
                            _hint = r >= _d && _d > r - 1;
                        }
                        if (_hint) {
                            covid19::GridPt _pt{ _center.x + i, _center.y + j };
                            if (entityGrids_.contains(_pt.x, _pt.y))
                                _ids->Add(entityGrids_.at(_pt.x, _pt.y));
                        }
                    }
            }
            _res.mutable_response()->set_code(bb::ResponseCode::Successed);
        });
}

std::vector<EntitySimModel::DescriptorEnum> MultiplySimModel::preconditionDescriptors() const
{
    return {
        bb::DescriptorType::GridInfo,
        bb::DescriptorType::Position,
        bb::DescriptorType::Contact
    };
}

bool MultiplySimModel::beforeStep(const GBBTimeContext& _timeContext)
{
    return amster::StatusID::SUCCESS
        == readSingltonDescriptorData(settings_);
}

StepResult MultiplySimModel::step(
    const GBBTimeContext& _timeContext,
    Entity& _entity)
{
    auto _elaspedSec = _timeContext.stepSecond();
    const auto& _durations = settings_.duration();
    const auto& _rates = settings_.rate();
    auto _id = _entity.entityID();
    bb::descriptor::GridInfo _info;
    _entity.readPublishedDescriptorData(_info);
    std::random_device _rd;
    std::mt19937 _gen(_rd());
    using bb::descriptor::IncubationDuration;
    using bb::descriptor::IncubationInfectDuration;
    using bb::descriptor::InfectDuration;
    using bb::descriptor::TreatmentDuration;
    using bb::descriptor::SpontaneousActiveDuration;

    bool _sync = settings_.sync_duration_time();
    switch (_info.type()) {
    case bb::GridType::Exposed:
        if (_sync ? stepDuration<IncubationDuration>(
                        _id,
                        _elaspedSec,
                        _durations.incubation())
                  : stepDuration(
                        _id,
                        bb::DurationOptions::Incubation,
                        _elaspedSec,
                        _durations.incubation()))
            if (_rates.incubation_infected()
                > std::generate_canonical<double, 10>(_gen))
                confirmDiagnosis(_entity);

        if (_sync ? stepDuration<IncubationInfectDuration>(
                        _id,
                        _elaspedSec,
                        _durations.incubation_infect())
                  : stepDuration(
                        _id,
                        bb::DurationOptions::IncubationInfect,
                        _elaspedSec,
                        _durations.incubation_infect()))
            spread(_entity, _rates.incubation_transmission());

        break;
    case bb::GridType::Infectious:
        if (_sync ? stepDuration<InfectDuration>(
                        _id,
                        _elaspedSec,
                        _durations.infect())
                  : stepDuration(
                        _id,
                        bb::DurationOptions::Infect,
                        _elaspedSec,
                        _durations.infect()))
            spread(_entity, _rates.transmission());

        if (_sync ? stepDuration<TreatmentDuration>(
                        _id,
                        _elaspedSec,
                        _durations.treatment())
                  : stepDuration(
                        _id,
                        bb::DurationOptions::Treatment,
                        _elaspedSec,
                        _durations.treatment()))
            recover(_entity);
        break;
    case bb::GridType::Susceptible:
        if (_sync ? stepDuration<SpontaneousActiveDuration>(
                        _id,
                        _elaspedSec,
                        _durations.spontaneous_activation())
                  : stepDuration(
                        _id,
                        bb::DurationOptions::SpontaneousActivation,
                        _elaspedSec,
                        _durations.spontaneous_activation()))
            if (_rates.spontaneous_activation()
                > std::generate_canonical<double, 10>(_gen))
                confirmDiagnosis(_entity);
        break;
    default:
        break;
    }
}

void MultiplySimModel::loopDestroyed()
{
    {
        absl::MutexLock _lk(&gridMutex_);
        entityGrids_ = {};
    }
    {
        absl::MutexLock _lk(&durationMutex_);
        entityDurations_ = {};
    }
    inited_ = false;
}

void MultiplySimModel::entityExisted(EntitySimModel::EntityCreationTuple _entityTuple)
{
    entityAdded(_entityTuple);
}

void MultiplySimModel::entityAdded(EntitySimModel::EntityCreationTuple _entityTuple)
{
    initFromBB();
    bb::descriptor::Position _pos;
    auto _id = std::get<0>(_entityTuple);
    readData(_id, _pos);
    auto _success = setGridEntity({ _pos.x(), _pos.y() }, _id);
    LOGF_IF(WARNING,
        !_success,
        "cell {}'{{{}}} is found in grids.",
        _id,
            _pos.DebugString());
}

void MultiplySimModel::createdCell(const gamelife::GridPt &pos, amster::gbbinfra::EntityId &metId)
{
    using namespace net::phoneyou::gamelife::bb;
    bb::message::NewCell _req;
    _req.set_cell_x(pos.x);
    _req.set_cell_y(pos.y);
    doRPC("New_Cell", _req ,
          [this]( const bb::message::NewCellId& _res , amster::gateway::RPCResultState _state ){
        if(amster::gateway::RPCResultState::Success != _state){
            return ;
            LOGF(WARNING , "rpc new cell {}" , _state);
        }

        metId = _res.met_id();

    });

}

void MultiplySimModel::multiplyCell()
{
    for(const auto& pos : cell_pos2id){
        pos.
    }
}

PLUMA_INHERIT_PROVIDER(MultiplySimModel, EntitySimModel)

PLUMA_CONNECTOR bool pluma_connect(pluma::Host& host)
{
    host.add(new MultiplySimModelProvider());
    return true;
}
