#ifndef MULTIPLYSIMMODEL_H
#define MULTIPLYSIMMODEL_H

#pragma once

#include <absl/synchronization/mutex.h>

#include <agent/entitysimmodel.h>
using namespace amster::agent;
using namespace amster::gbb;

#include <net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h>
namespace bb = net::phoneyou::gamelife::bb;

#include "common.h"


class MultiplySimModel : public EntitySimModel
{
public:
    MultiplySimModel();

    // EntitySimModel interface
protected:
    void initialize() override;
    std::vector<DescriptorEnum> preconditionDescriptors() const override;
    bool beforeStep(const amster::gbb::GBBTimeContext &_timeContext) override;
    StepResult step(const amster::gbb::GBBTimeContext &_timeContext, Entity &_entity) override;
    void loopDestroyed() override;
    void entityExisted(EntityCreationTuple _entityTuple) override;
    void entityAdded(EntityCreationTuple _entityTuple) override;
private:
    //创建细胞实体
    void createdCell(const gamelife::GridPt& pos , amster::EntityId& metId);
    //细胞死亡
    void deleteCell(const amster::EntityId& metId);
    //细胞繁衍
private:
    std::unordered_map<amster::EntityId , gamelife::GridPt > cell_id2pos;
    std::unordered_map< gamelife::GridPt , amster::EntityId > cell_pos2id;


};

#endif // MULTIPLYSIMMODEL_H
