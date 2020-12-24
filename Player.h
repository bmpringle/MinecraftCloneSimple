#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include "EventLib/Listener.h"
#include "Block.h"
#include "Model.h"
#include <chrono>

class World;

struct Pos {
    Pos(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

    }

    float x = 0;
    float y = 0;
    float z = 0;
};

class Player : public Listener, public Model {
    public:
        Player(World* _world);
        void updatePlayerInWorld(World* world);
        void listenTo(std::shared_ptr<Event> e);
        AABB getAABB();

    private:
        Pos pos;
        float metersPerSecond = 3;
        World* world;
        std::chrono::system_clock::time_point previousUpdate = std::chrono::system_clock::now();
        float currentYSpeed = 0;
};
#endif