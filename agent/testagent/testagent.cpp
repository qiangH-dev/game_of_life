﻿#include "testagent.h"
#include <random>
#include <unordered_set>

#include <gflags/gflags.h>
#include <g3log.hpp>

#include <agent/agentprovider.h>
#include <amster_global.h>
#include <gbb/gbbaccessor.h>

#include <net/phoneyou/gamelife/bb/Descriptors.pb.h>
#include <net/phoneyou/gamelife/bb/Messages.pb.h>
#include <net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h>

#include <amster/bb/Common.pb.h>
#include <amster/bb/Messages.pb.h>

TestAgent::TestAgent()
    : AgentBase("TestAgent")
{
}

void TestAgent::initialize()
{
//    amster::bb::message::EngineControl _control;
//    _control.set_commander(amster::bb::message::EngineControl_Commander_Stop);

//    publishMessage(_control);

    LOGF(DBUG , "test agent init");
    using namespace net::phoneyou::gamelife::bb;
    using namespace net::phoneyou::gamelife::bb::descriptor;

    //初始化网格
    bb::message::DimensionsInit _dimens;
    _dimens.set_cell_x(10);
    _dimens.set_cell_y(10);
    doRPC< bb::message::Response , bb::message::DimensionsInit>("Dimension_Init",_dimens ,
          [this , &_dimens](const bb::message::Response& _res ,
          amster::gateway::RPCResultState _state){

        if(_res.status() == bb::Status_Enum_Success)
            LOGF(DBUG , "init dimension cell {}*{}" , _dimens.cell_x(), _dimens.cell_y());
        else
            LOGF(DBUG , "init failed");
          });

    //创建entity
//    std::vector< bb::message::NewCell > vec;
    for(int i = 0; i < 4 ; ++i){
        bb::message::NewCell _cell;
        _cell.set_cell_x( 4+i );
        _cell.set_cell_y(5);
//        vec.push_back(_cell);
        doRPC<bb::message::NewCellId , bb::message::NewCell>("New_Cell" , _cell ,
            [this](const bb::message::NewCellId& _res ,
            amster::gateway::RPCResultState _state){

            LOGF(DBUG , "entity id:[{}] ", _res.met_id());
        });
    }




}

PLUMA_INHERIT_PROVIDER(TestAgent, AgentBase)

PLUMA_CONNECTOR bool pluma_connect(pluma::Host& host)
{
    host.add(new TestAgentProvider());
    return true;
}
