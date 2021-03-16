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

class Entity : public Model {
    public:
        Entity(World* _world);
        virtual void updateEntity(World* world);

        virtual AABB getAABB();
        virtual Pos getPos();
        virtual RenderedModel getRenderedModel();

        virtual void setItemInHandIndex(int index);
        virtual bool validatePosition(Pos newPosition, BlockArrayData* data);
        virtual bool isInWater(BlockArrayData* data);
        virtual bool canJumpInWater(BlockArrayData* data);
        virtual Inventory* getInventory();
        virtual int getItemInHandIndex();

        virtual bool isEntitySneaking();
        virtual bool isEntitySprinting();
        virtual bool isEntityInWater();

    protected:
        virtual bool validatePosition(Pos newPosition, BlockArrayData* data, float* yToSnapTo);
        virtual void move(glm::vec3* moveVec);
        virtual bool isBlockUnderEntity();

        World* world;

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

        Pos sneakPos = pos;

        Inventory inventory = Inventory(36);
};
#endif