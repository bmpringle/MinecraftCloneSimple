#ifndef WORLD_H
#define WORLD_H
#include "InputHandler.h"
#include "WorldRenderer.h"
#include "EventLib/EventQueue.h"
#include "BlockArrayData.h"
#include "Player.h"
#include "RenderInclude.h"

class World {
    public:
        World(GLFWwindow* window);

        void updateGame();

        void renderGame();

        void mainLoop();

        float getWorldGravity();

        BlockArrayData* getBlockData();
    private:
        void generateWorld();

        void internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        EventQueue worldEventQueue;
        InputHandler input;
        WorldRenderer renderer;
        BlockArrayData internalBlockData;
        GLFWwindow* window;
        std::shared_ptr<Player> thePlayer;

        //in meters/s^2
        float worldGravity = 2;
};

bool AABBIntersectedByAABB(AABB box1, AABB box2);

#endif