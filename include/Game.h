#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "RenderInclude.h"
#include "GameEventHandler.h"

class Game {
    public:
        Game(GLFWwindow* window);

        void pauseOrResume();

        void start();
    private:
        GLFWwindow* window;
        EventQueue eventQueue;
        InputHandler input;
        WorldRenderer renderer;
        World world;
        std::shared_ptr<GameEventHandler> gameEventHandler;

};
#endif