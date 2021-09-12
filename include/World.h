#ifndef WORLD_H
#define WORLD_H

#include "InputHandler.h"
#include "Renderer.h"
#include "EventQueue/EventQueue.h"
#include "BlockArrayData.h"
#include "Player.h"
#include "TimerMap/TimerMap.h"
#include "GameSettings.h"

class World {
    public:
        World(GLFWwindow* window, EventQueue* queue, InputHandler* inputHandler, Renderer* renderer, TimerMap* map, GameSettings* settings, std::string name, std::string worldFolder, int seed);

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

        Renderer* getRenderer();

        GameSettings* getSettings();

        std::string getName();
        
    private:
        void generateWorld();

        void updateGame();

        void renderGame();

        void renderOverlays();
        
        void dumpFrameTime();

        void cleanupHUD();

        std::string name;

        //these are all pointers because they cannot go out of scope before this World object.
        TimerMap* timerMap;
        EventQueue* worldEventQueue;
        InputHandler* input;
        Renderer* renderer;
        GLFWwindow* window;
        GameSettings* settings;

        BlockArrayData internalBlockData;
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