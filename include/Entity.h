#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include "EventQueue/Listener.h"
#include "Block.h"
#include "Model.h"
#include <chrono>
#include "BlockArrayData.h"
#include "ItemBlock.h"
#include "glmh.h"
#include "Pos.h"
#include "Inventory.h"
#include "InventoryGui.h"
#include "GameSettings.h"
#include "ObjLoader/ObjLoader.h"
#include "EntityVertex.h"

class Entity {
    public:
        Entity();
        virtual void updateEntity(World* world);

        virtual AABB getAABB();
        virtual Pos getPos();
        virtual void setPos(Pos p);
        virtual std::vector<EntityVertex> getRenderedModel();

        virtual void setItemInHandIndex(int index);
        virtual bool validatePosition(Pos newPosition, BlockArrayData* data);
        virtual bool isInWater(BlockArrayData* data);
        virtual bool canJumpInWater(BlockArrayData* data);
        virtual Inventory* getInventory();
        virtual int getItemInHandIndex();

        virtual bool isEntitySneaking();
        virtual bool isEntitySprinting();
        virtual bool isEntityInWater();

        void setID(int id);
        int getID();
        void setBufferedChunkLocation(BlockPos pos);

        virtual ~Entity() = default;

    protected:
        virtual bool validatePosition(Pos newPosition, BlockArrayData* data, float* yToSnapTo);
        virtual void move(glm::vec3* moveVec, World* world);
        virtual bool isBlockUnderEntity(World* world);

        Pos pos;

        int itemInHandIndex = 0;

        bool isGrounded = false;
        bool isJumping = false;

        bool isSneaking = false;
        bool isSprinting = false;
        
 
        bool waterPhysics = false;

        const float standingHeight = 1.8;
        const float sneakingHeight = 1.65;

        glm::vec3 motion = glm::vec3(0, 0, 0);

        BlockPos bufferedChunkLocation = BlockPos(0, 0, 0);

        Pos sneakPos = pos;

        Inventory inventory = Inventory(36);

        int id = 0;

        std::unique_ptr<ObjMesh> mesh;

        std::vector<EntityVertex> model;        
};
#endif