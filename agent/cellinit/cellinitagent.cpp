#include "cellinitagent.h".h"
#include <random>
#include <unordered_set>

#include <gflags/gflags.h>

#include <agent/agentprovider.h>

#include <net/phoneyou/gamelife/bb/Descriptors.pb.h>
#include <net/phoneyou/gamelife/bb/Messages.pb.h>
#include <net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h>

CellInitAgent::CellInitAgent()
    : AgentBase("CellInitAgent")
{
}

void CellInitAgent::initialize()
{
    using namespace net::phoneyou::gamelife::bb;
    using namespace net::phoneyou::gamelife::bb::descriptor;
    //初始化网格大小
    registerRPCService<bb::message::DimensionsInit , bb::message::Response>("Dimension_Init",
        [this](const bb::message::DimensionsInit& _req, bb::message::Response& _res){

        Dimensions _dimensions;
        _dimensions.set_cell_x(_req.cell_x());
        _dimensions.set_cell_y(_req.cell.y());

        CHECK_STATUS_EQ(writeSingltonDescriptorData(_dimensions));
        _res.set_status(bb::Status_Enum::Status_Enum_Success);
    });

    //创建cell
    registerRPCService<bb::message::NewCell , bb::message::NewCellId>("New_Cell",
        [this](const bb::message::NewCell& _req , bb::message::NewCellId& _res){

        EntityId _metId {};

        CHECK_STATUS_EXT(createAgentEntity(bb::EntityType::Cell, _metId), return );

        bb::descriptor::Position _cellPos;
        bb::descriptor::CellInfo _cellInfo;
        _cellPos.set_x(_req.cell_x());
        _cellPos.set_y(_req.cell_y());

        _cellInfo.set_type(bb::CellType_Enum::Status_Enum_Success);
        _cellInfo.set_life_cycle(0);


        CHECK_STATUS_EXT(writeData(_metId, _cellPos), return );
        CHECK_STATUS_EXT(writeData(_metId, _cellInfo), return );
        setEntityReady(_metId);

        _res.set_met_id(_metId);
    });


}



PLUMA_INHERIT_PROVIDER(CellInitAgent, AgentBase)

PLUMA_CONNECTOR bool pluma_connect(pluma::Host& host)
{
    host.add(new CellInitAgentProvider());
    return true;
}
