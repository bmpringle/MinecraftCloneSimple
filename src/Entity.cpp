#include "Entity.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "Blocks.h"
#include <thread> 

Entity::Entity(World* _world) : world(_world), pos(Pos(0, 40, 0)) {
    world->getTimerMap()->addTimerToMap("itemUseTimer");
}

void Entity::updateEntity(World* world) {  

    move(&motion);

    if(isInWater(world->getBlockData())) {
        waterPhysics = true;
    }else {
        waterPhysics = false;
    }

    if(!isBlockUnderEntity()) {
        motion[1] -= (0.086 / 6) * ((waterPhysics) ? 0.5 : 1);
        motion[1] *= 0.98;
    }

    if(abs(motion[0]) < 0.005) {
        motion[0] = 0;
    }else {
        motion[0] *= 0.9998;
    }

    if(abs(motion[2]) < 0.005) {
        motion[2] = 0;
    }else {
        motion[2] *= 0.98;
    }
}

AABB Entity::getAABB() {
    return AABB(pos.x, pos.y, pos.z, 0.6, (isSneaking) ? sneakingHeight : standingHeight, 0.6);
}

RenderedModel Entity::getRenderedModel() {
    RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p2 = RenderedPoint(0.6, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p3 = RenderedPoint(0.6, 0, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p4 = RenderedPoint(0, 0, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p5 = RenderedPoint(0, 1.8, 0, /**uv coords*/ 0, 0);
    RenderedPoint p6 = RenderedPoint(0.6, 1.8, 0, /**uv coords*/ 0, 0);
    RenderedPoint p7 = RenderedPoint(0.6, 1.8, 0.6, /**uv coords*/ 0, 0);
    RenderedPoint p8 = RenderedPoint(0, 2, 0.6, /**uv coords*/ 0, 0);

    RenderedTriangle t1 = RenderedTriangle(p1, p2, p3, 1);
    RenderedTriangle t2 = RenderedTriangle(p1, p4, p3, 0);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p7, 1);
    RenderedTriangle t4 = RenderedTriangle(p5, p8, p7, 0);
    RenderedTriangle t5 = RenderedTriangle(p1, p4, p8, 1);
    RenderedTriangle t6 = RenderedTriangle(p1, p5, p8, 1);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p7, 0);
    RenderedTriangle t8 = RenderedTriangle(p2, p6, p7, 1);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5, 0);
    RenderedTriangle t10 = RenderedTriangle(p2, p5, p6, 1);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p8, 0);
    RenderedTriangle t12 = RenderedTriangle(p3, p7, p8, 1);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }

    return RenderedModel(triangles);
}

Pos Entity::getPos() {
    return pos;
}

bool Entity::validatePosition(Pos newPosition, BlockArrayData* data) {
    AABB entityAABB = getAABB();
    float a;
    return data->isValidPosition(entityAABB, &a);
}

bool Entity::validatePosition(Pos newPosition, BlockArrayData* data, float* yToSnapTo) {
    AABB entityAABB = getAABB();
    return data->isValidPosition(entityAABB, yToSnapTo);
}

void Entity::setItemInHandIndex(int index) {
    itemInHandIndex = index;
}

void Entity::move(glm::vec3* moveVec) {    
    double d3 = (*moveVec)[0];
    double d4 = (*moveVec)[1];
    double d5 = (*moveVec)[2];

    Pos previousPos = pos;

    pos.x += d3;

    if(!this->validatePosition(pos, world->getBlockData())) {
        pos = previousPos;
    }

    previousPos = pos;

    pos.z += d5;

    if(!this->validatePosition(pos, world->getBlockData())) {
        pos = previousPos;
    }

    previousPos = pos;

    float yPosToSnapTo[1] = {pos.y};

    pos.y += d4;

    if(!this->validatePosition(pos, world->getBlockData(), yPosToSnapTo)) {
        pos = previousPos;
        if(d4 < 0) {
            pos.y = yPosToSnapTo[0];
            isJumping = false;
        }
        (*moveVec)[1] = 0;
    }

    bool flag = isBlockUnderEntity();

    if(!flag && isGrounded && !isJumping && isSneaking) {
        pos = sneakPos;

        (*moveVec)[1] = 0;
    }else if(!isJumping && isSneaking && flag) {
        sneakPos = pos;
    }

    flag = isBlockUnderEntity();

    if(!flag) {
        isGrounded = false;
    }else {
        isGrounded = true;
    }
}

bool Entity::isBlockUnderEntity() {
    pos.y -= 0.002;
    if(!this->validatePosition(pos, world->getBlockData())) {
        pos.y += 0.002;
        return true;
    }else {
        pos.y += 0.002;
        return false;
    }
}

bool Entity::canJumpInWater(BlockArrayData* data) {
    AABB aabb = getAABB();
    aabb.startY += 0.4;
    aabb.ySize -= 0.4;
    return data->isAABBInWater(aabb);
}

bool Entity::isInWater(BlockArrayData* data) {
    AABB aabb = getAABB();
    aabb.ySize += 0.05;
    return data->isAABBInWater(aabb);
}

Inventory* Entity::getInventory() {
    return &inventory;
}

int Entity::getItemInHandIndex() {
    return itemInHandIndex;
}

bool Entity::isEntitySneaking() {
    return isSneaking;
}

bool Entity::isEntitySprinting() {
    return isSprinting;
}

bool Entity::isEntityInWater() {
    return waterPhysics;
}