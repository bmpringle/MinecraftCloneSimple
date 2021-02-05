#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "EventLib/Listener.h"
#include "Block.h"
#include "Model.h"
#include <chrono>
#include "BlockArrayData.h"
#include "ItemBlock.h"

class World;

class Pos {
    public:
        Pos(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

        }

        float x = 0;
        float y = 0;
        float z = 0;

        friend Pos operator+(Pos lhs, Pos rhs) {
            return Pos(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        }
};

class MoveVec2 {
    public:
        MoveVec2() {

        }
        float relativeX = 0;
        float relativeZ = 0;
        double durationInMilliseconds = 0;
};

class Player final : public Listener, public Model {
    public:
        Player(World* _world);
        void updatePlayerInWorld(World* world);
        void listenTo(std::shared_ptr<Event> e);
        AABB getAABB();
        Pos getPos();
        RenderedModel getRenderedModel();
        double getXRotation();
        double getYRotation();
        Pos getCameraPosition();
        Pos getCameraNormal();
        BlockPos* getBlockLookingAt();
        int getSideOfBlockLookingAt();
        void setItemInHand(std::unique_ptr<Item> item);
        bool validatePosition(Pos newPosition, BlockArrayData data);
    private:
        bool validatePosition(Pos newPosition, BlockArrayData data, float* yToSnapTo);
        void updatePlayerLookingAt(World* world);
        float raycast(AABB box, int* side);

        Pos pos;

        //in m/s
        float speed = 4.3;
        World* world;
        float currentYSpeed = 0;

        float yaw = 0;
        float pitch = 0;

        MoveVec2 moveVector = MoveVec2();

        BlockPos* blockLookingAt = nullptr;

        //this exists to prevent the pointer you should use from going stale DO NOT READ THIS POINTER
        BlockPos internalBlockLookingAt = BlockPos(0, 0, 0);

        //order 1: lowX, 2: highX, 3: bottomY, 4: topY, 5: lowZ, 6: highZ
        int sideOfBlockLookingAt = 0;

        std::unique_ptr<Item> itemInHand = nullptr;
};
#endif