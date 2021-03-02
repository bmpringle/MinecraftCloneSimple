#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "RenderInclude.h"
#include "GameEventHandler.h"
#include "MainMenuGui.h"

class Game {
    public:
        Game(GLFWwindow* window);

        void pauseOrResume();

        void start();

        MainMenuGui* getGui();

        void internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void internalMouseCallback(GLFWwindow* window, double xpos, double ypos);
        
        void internalMouseButtonCallback(GLFWwindow* w, int button, int action, int mods);

        void quitToMainMenu();
    private:
        GLFWwindow* window;
        EventQueue eventQueue;
        TimerMap map;
        InputHandler input;
        WorldRenderer renderer;
        World* world;
        std::shared_ptr<GameEventHandler> gameEventHandler;

        MainMenuGui gui;

};
#endif