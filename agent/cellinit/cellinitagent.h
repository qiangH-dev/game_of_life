#ifndef CELLINIT_H
#define CELLINIT_H

#pragma once

#include <agent/agentbase.h>
using namespace amster::agent;
using namespace amster::gbb;

#include <net/phoneyou/gamelife/bb/Common.pb.h>
namespace bb = net::phoneyou::gamelife::bb;

class CellInitAgent : public AgentBase {
public:
    CellInitAgent();

    // AgentBase interface
public:
    void initialize() override;

};

#endif // CELLINIT_H
