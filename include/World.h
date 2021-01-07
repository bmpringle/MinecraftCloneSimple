#ifndef WORLD_H
#define WORLD_H
#include "InputHandler.h"
#include "WorldRenderer.h"
#include "EventLib/EventQueue.h"
#include "BlockArrayData.h"
#include "Player.h"
#include "RenderInclude.h"
#include "TimerMapLib/TimerMap.h"

class World {
    public:
        World(GLFWwindow* window, EventQueue* queue, InputHandler* inputHandler);

        void mainLoop();

        float getWorldGravity();

        BlockArrayData* getBlockData();

        std::shared_ptr<Player> getPlayer();

        TimerMap* getTimerMap();

        bool isPaused();

        void resume();

        void pause();
    private:
        void generateWorld();

        void updateGame();

        void renderGame();

        void internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void internalMouseCallback(GLFWwindow* window, double xpos, double ypos);
        
        void internalMouseButtonCallback(GLFWwindow* w, int button, int action, int mods);
        
        TimerMap timerMap;
        EventQueue* worldEventQueue;
        InputHandler* input;
        WorldRenderer renderer;
        BlockArrayData internalBlockData;
        GLFWwindow* window;
        std::shared_ptr<Player> thePlayer;
        bool paused = false;

        //in meters/s^2
        float worldGravity = 32;
};

bool AABBIntersectedByAABB(AABB box1, AABB box2);

#endif