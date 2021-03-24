#include "Entity.h"
#include "World.h"
#include <iostream>
#include "Events.h"
#include <math.h>
#include "Blocks.h"
#include <thread> 
#include "EntityVertex.h"

Entity::Entity() : pos(Pos(0, 40, 0)), mesh(nullptr) {
    ObjMeshPrimitive prim = ObjLoader::loadMeshFromFile("src/assets/models/entity/high_quality_teapot.obj");
    ObjLoader::triangulatePrimitiveMesh(prim);
    std::vector<ObjMeshPrimitive> prims = std::vector<ObjMeshPrimitive>();
    prims.push_back(prim);
    mesh = ObjLoader::combinePrimitives(prims);
    for(ObjMeshPrimitive& pmesh : mesh->submeshes) {
        //we assume that obj files are already triangulated.
        for(ObjFace& face : pmesh.faces) {
            for(int i = 0; i < face.indicesVG.size(); ++i) {
                GeometricVertex vg = pmesh.geometricVertices.at(face.indicesVG[i]);
                vg.geometricVertex.x = vg.geometricVertex.x * 0.01;
                vg.geometricVertex.y = vg.geometricVertex.y * 0.01;
                vg.geometricVertex.z = vg.geometricVertex.z * 0.01;

                TextureVertex vt;
                if(face.indicesVT.size() > 0) {
                    vt = pmesh.textureVertices.at(face.indicesVT[i]);
                }else {
                    vt = TextureVertex();
                }
                model.push_back(EntityVertex(vg.geometricVertex, glm::vec2(vt.textureVertex[0], vt.textureVertex[1])));
            }
        }
    }
}

void Entity::updateEntity(World* world) {  
    //motion[2] = 0.05;
    move(&motion, world);

    if(isInWater(world->getBlockData())) {
        waterPhysics = true;
    }else {
        waterPhysics = false;
    }
 
    if(!isBlockUnderEntity(world)) {
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

    if(bufferedChunkLocation != world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates() && !world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->isFakeChunk()) {
        std::map<int, std::shared_ptr<Entity>>& entities = world->getBlockData()->getChunkWithBlock(bufferedChunkLocation)->getEntitiesInChunk();
        std::map<int, std::shared_ptr<Entity>>& entitiesNew = world->getBlockData()->getChunkWithBlock(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates())->getEntitiesInChunk();
        entitiesNew[id] = entities[id];
        entities.erase(id);
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates());
    }else if(!world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->isFakeChunk()){
        setBufferedChunkLocation(world->getBlockData()->getChunkWithBlock(getPos().toBlockPos())->getChunkCoordinates());
    }
}

AABB Entity::getAABB() {
    return AABB(pos.x, pos.y, pos.z, 0.6, (isSneaking) ? sneakingHeight : standingHeight, 0.6);
}

std::vector<EntityVertex> Entity::getRenderedModel() {
    return model;
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

void Entity::move(glm::vec3* moveVec, World* world) {    
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

    bool flag = isBlockUnderEntity(world);

    if(!flag && isGrounded && !isJumping && isSneaking) {
        pos = sneakPos;

        (*moveVec)[1] = 0;
    }else if(!isJumping && isSneaking && flag) {
        sneakPos = pos;
    }

    flag = isBlockUnderEntity(world);

    if(!flag) {
        isGrounded = false;
    }else {
        isGrounded = true;
    }
}

bool Entity::isBlockUnderEntity(World* world) {
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

void Entity::setPos(Pos p) {
    pos = p;
}

void Entity::setID(int id) {
    this->id = id;
}

int Entity::getID() {
    return id;
}

void Entity::setBufferedChunkLocation(BlockPos pos) {
    this->bufferedChunkLocation = pos;
}