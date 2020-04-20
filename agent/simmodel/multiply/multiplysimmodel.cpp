#include "multiplysimmodel.h"
#include <random>
#include <unordered_map>

#include <agent/agentprovider.h>
#include <gateway/gateway_constants.h>
#include <gbbinfra/descriptorregister.h>

#include <net/phoneyou/gamelife/bb/Descriptors.pb.h>
#include <net/phoneyou/gamelife/bb/Messages.pb.h>



MultiplySimModel::MultiplySimModel()
    : EntitySimModel("MultiplySim")
{
    LOGF(DBUG , "constructor multiply sim");
}

void MultiplySimModel::initialize()
{
    LOGF(DBUG , "multiply sim init");

    using namespace net::phoneyou::gamelife::bb;
    registerRPCService<bb::message::NewCell , bb::message::NewCellId >("New_Cell",
        [this](const bb::message::NewCell& _req , bb::message::NewCellId& _res){

        EntityId _metId;
        gamelife::GridPt pt{ (int)_req.cell_x() , (int)_req.cell_y() };
        createdCell( pt , _metId);

        _res.set_met_id(_metId);

    });

    registerRPCService<bb::message::DelCell , bb::message::DelCellRes>("Del_Cell",
                                                                       [this](const bb::message::DelCell& _req , bb::message::DelCellRes& _res){

        if(_req.met_id() != 0){
            deleteCell( _req.met_id() );
            _res.set_met_id(_req.met_id());
        }else{
            gamelife::GridPt _pt {(int)_req.pos().x() , (int)_req.pos().y()};
            deleteCell( _pt );
            _res.CopyFrom( _req.pos() );
        }

    });

}

std::vector<EntitySimModel::DescriptorEnum> MultiplySimModel::preconditionDescriptors() const
{
    return {
        bb::DescriptorType::CellInfo,
        bb::DescriptorType::Position
    };
}

bool MultiplySimModel::beforeStep(const GBBTimeContext& _timeContext)
{
    return true;
}

StepResult MultiplySimModel::step(
    const GBBTimeContext& _timeContext,
    Entity& _entity)
{
    LOGF(DBUG , "multiply sim step");

    multiplyCell();

    return StepResult::Done;
}

void MultiplySimModel::loopDestroyed()
{

}

void MultiplySimModel::entityExisted(EntitySimModel::EntityCreationTuple _entityTuple)
{

}

void MultiplySimModel::entityAdded(EntitySimModel::EntityCreationTuple _entityTuple)
{

}

void MultiplySimModel::createdCell(gamelife::GridPt &_cell, amster::gbbinfra::EntityId &_metId)
{

    createAgentEntity(bb::EntityType::Cell, _metId);

    bb::descriptor::Position _cellPos;
    bb::descriptor::CellInfo _cellInfo;
    _cellPos.set_x(_cell.x);
    _cellPos.set_y(_cell.y);

    _cellInfo.set_type(bb::CellType_Enum::CellType_Enum_Survive);
    _cellInfo.set_life_cycle(0);


    writeData(_metId, _cellPos);
    writeData(_metId, _cellInfo);
    setEntityReady(_metId);

    //cell postion 于 entityid 的映射
    gamelife::Grid<gamelife::GridPt> _grid{ uint32_t(_cell.x) , uint32_t(_cell.y) , _cell.around8() };
    cell_pos2id.insert( {_cell , _metId} );
    cell_id2Grid.insert(std::make_pair(_metId , _grid));

    LOGF(DBUG , "entity id:[], pos:({},{})" , _metId , _cell.x, _cell.y);

}

void MultiplySimModel::deleteCell(const amster::gbbinfra::EntityId &_metId)
{
    deleteEntity(_metId);

    //同步数据
    auto it = cell_id2Grid.find( _metId );
    gamelife::GridPt pt{ int(it->second.x()) , int(it->second.y()) };
    cell_pos2id.erase( pt );
    cell_id2Grid.erase( _metId );

}

void MultiplySimModel::deleteCell(const gamelife::GridPt &_pt)
{
    EntityId _metId;
    auto it = cell_pos2id.find( _pt );
    _metId = it->second;
    gamelife::Grid<gamelife::GridPt> _grid {(uint32_t)_pt.x , (uint32_t)_pt.y , _pt.around8() };
    cell_pos2id.erase( it );
    cell_id2Grid.erase( _metId );

    deleteEntity(_metId);

}

void MultiplySimModel::multiplyCell()
{

    std::map< EntityId , gamelife::GridPt > addCellId2posMap;

    for(auto& _cell : cell_id2Grid ){
        auto type_cell =cellType( _cell.second.x() , _cell.second.y());
        if(type_cell == CELL_TYPE::DEATH){ //当前cell 已经是Survive 时，只处理death情况
            //TODO: delete cell
            deleteCell(_cell.first);
        }
        //判断邻居 cell
        for(auto around_cell : _cell.second.data()){
            if(!contains(around_cell)){//判断邻居cell 是否为 Suivive状态，是：不处理(不做重复操作)，否：处理
                auto around_type = cellType(around_cell);
                if(CELL_TYPE::MULTIPLY == around_type ){//只有非suivive状态的邻居cell 为 MULTIPLY 时才处理
                    //TODO:created cell
                    EntityId _metId;
                    createdCell(around_cell , _metId );
                    addCellId2posMap.insert( std::make_pair( _metId , around_cell ) ); //需添加的cell map
                }
            }

        }

    }

    //更新数据
    for(auto& _cell : addCellId2posMap){
        gamelife::Grid<gamelife::GridPt> _grid{ (uint32_t)_cell.second.x , (uint32_t)_cell.second.y , _cell.second.around8()};
        cell_id2Grid.insert( std::make_pair(_cell.first , _grid) );
        cell_pos2id.insert( {_cell.second , _cell.first} );
    }


}

bool MultiplySimModel::contains(uint32_t _x, uint32_t _y)
{
    gamelife::GridPt _pt{(int)_x , (int)_y};
    return contains(_pt);
}

bool MultiplySimModel::contains(const gamelife::GridPt& _pt)
{
    auto it = cell_pos2id.find(_pt); //判断pt cell 是否在 存活的集合中

    return it != cell_pos2id.end();
}

MultiplySimModel::CELL_TYPE MultiplySimModel::cellType(uint32_t _x, uint32_t _y)
{
    gamelife::GridPt _pt{ (int)_x , (int)_y};
    return cellType(_pt);

}

MultiplySimModel::CELL_TYPE MultiplySimModel::cellType(const gamelife::GridPt &_pt)
{
    CELL_TYPE result{CELL_TYPE::DEATH};
    uint32_t type {0};
    auto around_vec = _pt.around8();
    for(auto& _cell : around_vec)
        if(contains(_cell)){ ++type; }; //邻居cell 存在 survive cell时 加一

    switch(type){
    case 2:
        result = CELL_TYPE::SURVIVE;
        break;
    case 3:
        result = CELL_TYPE::MULTIPLY;
        break;
    default:
        result = CELL_TYPE::DEATH;

    }

    return result;
}

PLUMA_INHERIT_PROVIDER(MultiplySimModel, EntitySimModel)

PLUMA_CONNECTOR bool pluma_connect(pluma::Host& host)
{
    host.add(new MultiplySimModelProvider());
    return true;
}
