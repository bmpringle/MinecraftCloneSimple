#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "EventLib/Listener.h"
#include "Block.h"
#include "Model.h"
#include <chrono>
#include "BlockArrayData.h"
#include "ItemBlock.h"
#include "glmh.h"
#include "Pos.h"
#include "Inventory.h"

class World;

class Player final : public Listener, public Model {
    public:
        Player(World* _world);
        void updateClient(World* world);
        void listenTo(std::shared_ptr<Event> e);
        AABB getAABB();
        Pos getPos();
        RenderedModel getRenderedModel();
        double getXRotation();
        double getYRotation();
        Pos getCameraPosition();
        Pos getCameraNormal();
        BlockPos* getBlockLookingAt();
        SideEnum getSideOfBlockLookingAt();
        void setItemInHandIndex(int index);
        bool validatePosition(Pos newPosition, BlockArrayData* data);
        void setBufferedChunkLocation(BlockPos pos);
        void updateServer(World* _world);
        bool isInWater(BlockArrayData* data);
        bool canJumpInWater(BlockArrayData* data);
        Inventory* getInventory();
        int getItemInHandIndex();
        
    private:
        bool validatePosition(Pos newPosition, BlockArrayData* data, float* yToSnapTo);
        void updatePlayerLookingAt(World* world);
        float raycast(AABB box, SideEnum* side);
        void updateHorizontalMotion();
        void move(glm::vec3* moveVec);
        bool isBlockUnderPlayer();

        Pos pos;

        //in m/s
        float speed = 4.3;
        World* world;

        float yaw = 0;
        float pitch = 0;

        BlockPos* blockLookingAt = nullptr;

        //this exists to prevent the pointer you should use from going stale DO NOT READ THIS POINTER
        BlockPos internalBlockLookingAt = BlockPos(0, 0, 0);

        SideEnum sideOfBlockLookingAt = NORTH;

        int itemInHandIndex = 0;

        BlockPos bufferedChunkLocation = BlockPos(0, 0, 0);

        bool isGrounded = false;
        bool isJumping = false;

        bool isSneaking = false;
        bool isSprinting = false;
        
 
        bool waterPhysics = false;

        const float standingHeight = 1.8;
        const float sneakingHeight = 1.65;

        glm::vec3 motion = glm::vec3(0, 0, 0);

        int xInputDirection = 0;
        int zInputDirection = 0;

        Pos sneakPos = pos;

        Inventory inventory = Inventory(36);
};
#endif