#ifndef TESTAGENT_H
#define TESTAGENT_H

#pragma once

#include <agent/agentbase.h>
using namespace amster::agent;
using namespace amster::gbb;

#include <net/phoneyou/gamelife/bb/Common.pb.h>
namespace bb = net::phoneyou::gamelife::bb;

class TestAgent : public AgentBase {
public:
    TestAgent();

    // AgentBase interface
public:
    void initialize() override;

};

#endif // TESTAGENT_H
