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
        World(GLFWwindow* window, EventQueue* queue, InputHandler* inputHandler, WorldRenderer* renderer, TimerMap* map);

        void mainLoop();

        float getWorldGravity();

        BlockArrayData* getBlockData();

        std::shared_ptr<Player> getPlayer();

        TimerMap* getTimerMap();

        bool isPaused();

        void resume();

        void pause();

        void quit();

        int getChunkRenderDistance();

        GLFWwindow* getWindowPtr();

        WorldRenderer* getWorldRenderer();
        
    private:
        void generateWorld();

        void updateGame();

        void renderGame();

        void renderOverlays();
        
        void dumpFrameTime();

        TimerMap* timerMap;
        EventQueue* worldEventQueue;
        InputHandler* input;
        WorldRenderer* renderer;
        BlockArrayData internalBlockData;
        GLFWwindow* window;
        std::shared_ptr<Player> thePlayer;
        bool paused = false;
        bool quitting = false;

        int chunkRenderDistance = 4;

        std::chrono::high_resolution_clock::time_point prevFrameTimePoint = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point secondTimer = std::chrono::high_resolution_clock::now();
        double frameTimeSum = 0;
        int frameTimeCounter = 0;

        float worldGravity = 32;
};

bool AABBIntersectedByAABB(AABB box1, AABB box2);
bool AABBIntersectedByAABBVocal(AABB box1, AABB box2);

#endif