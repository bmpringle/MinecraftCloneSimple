#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "RenderInclude.h"

class Game;

class GameEventHandler : public Listener {
    public:
        GameEventHandler(Game* game);

        void listenTo(std::shared_ptr<Event> e);
    private:
        Game* game;
};

class Game {
    public:
        Game(GLFWwindow* window);

        void pauseOrResume();

        void start();
    private:
        GLFWwindow* window;
        EventQueue eventQueue;
        InputHandler input;
        World world;
        std::shared_ptr<GameEventHandler> gameEventHandler;

};
#endif