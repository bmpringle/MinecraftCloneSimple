#include "Game.h"
#include "MainMenuGui.h"

Game::Game(GLFWwindow* _window) : window(_window), eventQueue(EventQueue()), map(TimerMap()), input(InputHandler()), renderer(WorldRenderer()), world(nullptr), gameEventHandler(std::make_shared<GameEventHandler>(GameEventHandler(this))), gui(MainMenuGui(&renderer)) {
    eventQueue.addEventListener(gameEventHandler);

    glfwSetWindowUserPointer(window, this);

    auto func = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalKeyCallback(w, key, scancode, action, mods);
    };

    auto func2 = [](GLFWwindow* w, double xpos, double ypos) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalMouseCallback(w, xpos, ypos);
    };

    auto func3 = [](GLFWwindow* w, int button, int action, int mods) {
        static_cast<Game*>(glfwGetWindowUserPointer(w))->internalMouseButtonCallback(w, button, action, mods);
    };

    glfwSetKeyCallback(window, func);
    glfwSetCursorPosCallback(window, func2);
    glfwSetMouseButtonCallback(window, func3);
}

void Game::start() {    
    while(!glfwWindowShouldClose(window)) {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.updateAspectRatio(window);

        gui.displayGui(&renderer, 0, 0);

        if(gui.singleplayer.isPressed()) {
            if(world != nullptr) {
                delete(world);
            }
            world = new World(window, &eventQueue, &input, &renderer, &map);
            world->resume();
        }
        
        if(gui.quit.isPressed()){
            break;
        }

        input.callRegularEvents(&eventQueue, &map);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::pauseOrResume() {
    if(world != nullptr) {
        if(world->isPaused()) {
            world->resume();
        }else {
            world->pause();
        } 
    }
}

MainMenuGui* Game::getGui() {
    return &gui;
}

void Game::internalKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    input.handleInput(window, key, scancode, action, mods, &eventQueue, &map);
}

void Game::internalMouseCallback(GLFWwindow* window, double xpos, double ypos) {
    input.handleMouseInput(window, xpos, ypos, &eventQueue, &map);
}

void Game::internalMouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
    input.handleMouseButtonInput(w, button, action, mods, &eventQueue, &map);
}

void Game::quitToMainMenu() {
    world->quit();
}