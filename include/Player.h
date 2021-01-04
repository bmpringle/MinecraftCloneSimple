#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "EventLib/Listener.h"
#include "Block.h"
#include "Model.h"
#include <chrono>
#include "BlockArrayData.h"

class World;

struct Pos {
    Pos(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

    }

    float x = 0;
    float y = 0;
    float z = 0;
};

struct MoveVec2 {
    MoveVec2() {

    }
    float relativeX = 0;
    float relativeZ = 0;
    double durationInMilliseconds = 0;
};

class Player : public Listener, public Model {
    public:
        Player(World* _world);
        void updatePlayerInWorld(World* world);
        void listenTo(std::shared_ptr<Event> e);
        AABB getAABB();
        Pos getPos();
        RenderedModel getRenderedModel();
        double getXRotation();
        double getYRotation();

    private:
        bool validatePosition(Pos newPosition, BlockArrayData data);
        bool validatePosition(Pos newPosition, BlockArrayData data, float* yToSnapTo);

        Pos pos;

        //in m/s
        float speed = 4.3;
        World* world;
        float currentYSpeed = 0;

        float yaw = 0;
        float pitch = 0;
        float roll = 0;

        MoveVec2 moveVector = MoveVec2();

};
#endif