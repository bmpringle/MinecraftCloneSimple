#include "GameEventHandler.h"
#include "Events.h"
#include "Game.h"

GameEventHandler::GameEventHandler(Game* _game) : game(_game) {

}

void GameEventHandler::listenTo(std::shared_ptr<Event> e) {
    if(e->getEventID() == "KEYPRESSED") {
        KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());

        if(keyEvent.key == "ESC") {
            game->quitToMainMenu();  
        }
    }

    if(e->getEventID() == "MOUSEPOS") {
        MousePosEvent mouseEvent = *dynamic_cast<MousePosEvent*>(e.get());
        mouseX = mouseEvent.x;
        mouseY = mouseEvent.y;
    }

    if(e->getEventID() == "LEFTMOUSEBUTTONPRESSED") {
        if(game->getGui() != nullptr) {
            game->getGui()->mouseClick(mouseX, mouseY);
        }
    }
}