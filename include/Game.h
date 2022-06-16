#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "GameEventHandler.h"
#include "MainMenuGui.h"
#include "GameSettings.h"

class Game {
    public:
        Game(GLFWwindow* window);

        void pauseOrResume();

        void start();

        std::shared_ptr<Gui> getGui();

        void internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void internalMouseCallback(GLFWwindow* window, double xpos, double ypos);
        
        void internalMouseButtonCallback(GLFWwindow* w, int button, int action, int mods);

        void internalScrollCall(GLFWwindow* window, double offsetX, double offsetY);

        void quitToMainMenu();
    private:
        EventQueue eventQueue;
        TimerMap map;
        InputHandler input;
        VulkanRenderer renderer;
        std::shared_ptr<World> world;
        std::shared_ptr<GameEventHandler> gameEventHandler;

        std::shared_ptr<Gui> gui;
        GameSettings settings;

};
#endif