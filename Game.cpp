#include "Game.h"
#include "Events.h"

Game::Game(GLFWwindow* _window) : window(_window), eventQueue(EventQueue()), input(InputHandler()), world(World(_window, &eventQueue, &input)), gameEventHandler(std::make_shared<GameEventHandler>(GameEventHandler(this))) {
    eventQueue.addEventListener(gameEventHandler);
}

void Game::start() {
    world.resume();
    while(!glfwWindowShouldClose(window)) {
        input.callRegularEvents(&eventQueue);
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

GameEventHandler::GameEventHandler(Game* _game) : game(_game) {

}

void GameEventHandler::listenTo(std::shared_ptr<Event> e) {
    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());
        if(keyEvent.key == 'p') {
            game->pauseOrResume();  
        }
    }
}