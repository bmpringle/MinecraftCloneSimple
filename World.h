#ifndef WORLD_H
#define WORLD_H
#include "InputHandler.h"
#include "WorldRenderer.h"
#include "EventLib/EventQueue.h"
#include "BlockArrayData.h"
#include "Player.h"

class World {
    public:
        World();

        void updateGame();

        void renderGame();
    private:
        void generateWorld();

        Player thePlayer;
        InputHandler input;
        WorldRenderer renderer;
        EventQueue worldEventQueue;
        BlockArrayData internalBlockData;
};
#endif