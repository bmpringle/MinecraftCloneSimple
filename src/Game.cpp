#include "Game.h"

Game::Game(GLFWwindow* _window) : window(_window), eventQueue(EventQueue()), input(InputHandler()), world(World(_window, &eventQueue, &input)), gameEventHandler(std::make_shared<GameEventHandler>(GameEventHandler(this))) {
    eventQueue.addEventListener(gameEventHandler);
}

void Game::start() {
    world.resume();
    
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        input.callRegularEvents(&eventQueue, world.getTimerMap());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::pauseOrResume() {
    if(world.isPaused()) {
        world.resume();
    }else {
        world.pause();
    } 
}