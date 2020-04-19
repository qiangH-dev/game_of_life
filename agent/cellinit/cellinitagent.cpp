#include "cellinitagent.h".h"
#include <random>
#include <unordered_set>

#include <gflags/gflags.h>

#include <agent/agentprovider.h>
#include <amster_global.h>

#include <net/phoneyou/gamelife/bb/Descriptors.pb.h>
#include <net/phoneyou/gamelife/bb/Messages.pb.h>
#include <net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h>

CellInitAgent::CellInitAgent()
    : AgentBase("CellInit")
{
}

void CellInitAgent::initialize()
{
    using namespace net::phoneyou::gamelife::bb;
    using namespace net::phoneyou::gamelife::bb::descriptor;
    //初始化网格大小
    registerRPCService<bb::message::DimensionsInit , bb::message::Response>("Dimension_Init",
        [this](const bb::message::DimensionsInit& _req, bb::message::Response& _res){

        bb::descriptor::Dimensions _dimensions;
        _dimensions.set_cell_x(_req.cell_x());
        _dimensions.set_cell_y(_req.cell_y());

        amster::StatusID status = writeSingltonDescriptorData(_dimensions);
        if(status == amster::StatusID::SUCCESS)
            _res.set_status(bb::Status_Enum::Status_Enum_Success);
        else
            _res.set_status(bb::Status_Enum::Status_Enum_Failed);
    });

    //创建cell


}

PLUMA_INHERIT_PROVIDER(CellInitAgent, AgentBase)

PLUMA_CONNECTOR bool pluma_connect(pluma::Host& host)
{
    host.add(new CellInitAgentProvider());
    return true;
}
