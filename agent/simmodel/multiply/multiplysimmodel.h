#ifndef MULTIPLYSIMMODEL_H
#define MULTIPLYSIMMODEL_H

#pragma once

#include <shared_mutex>

#include <agent/entitysimmodel.h>
using namespace amster::agent;
using namespace amster::gbb;

#include <net/phoneyou/gamelife/bb/SingletonDescriptors.pb.h>
namespace bb = net::phoneyou::gamelife::bb;

#include "common.h"


class MultiplySimModel : public EntitySimModel
{
private:
    enum CELL_TYPE{ DEATH = 0 , SURVIVE = 1, MULTIPLY = 2}; //cell status type
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
    void createdCell(gamelife::GridPt &_cell , amster::EntityId& _metId);
    void createdCell(gamelife::GridPt &_cell);
    //细胞死亡
    void deleteCell(const amster::EntityId& _metId);
    void deleteCell(const gamelife::GridPt& _pt);
    //细胞繁衍
    void multiplyCell();

    //
    bool contains(uint32_t _x , uint32_t _y);
    bool contains(const gamelife::GridPt &_pt);

    CELL_TYPE cellType(uint32_t _x , uint32_t _y);
    CELL_TYPE cellType(const gamelife::GridPt& _pt);

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<amster::EntityId , gamelife::Grid<gamelife::GridPt> > cell_id2Grid;
//    std::unordered_map<amster::EntityId , gamelife::GridPt > cell_id2pos;
    std::map< gamelife::GridPt , amster::EntityId > cell_pos2id;


};

#endif // MULTIPLYSIMMODEL_H
