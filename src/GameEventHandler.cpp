#include "GameEventHandler.h"
#include "Events.h"
#include "Game.h"
#include "OptionsGui.h"
#include "SingleplayerSelectGui.h"

GameEventHandler::GameEventHandler(Game* _game) : game(_game) {

}

void GameEventHandler::listenTo(std::shared_ptr<Event> e) {
    if(game->getGui() != nullptr) {
        if(game->getGui()->getID() == 2) {
            std::shared_ptr<OptionsGui> optionsGui = std::dynamic_pointer_cast<OptionsGui>(game->getGui());
            if(e->getEventID() != "KEYPRESSED" && (e->getEventID() == "LEFT_CLICK" || e->getEventID() == "RIGHT_CLICK")) {
                optionsGui->handleOptionInput(e->getEventID());
            }else if(e->getEventID() == "KEYPRESSED") {
                KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());
                optionsGui->handleOptionInput(keyEvent.key);
            }
        } else if(game->getGui()->getID() == 3) {
            if(e->getEventID() == "KEYPRESSED") {
                KeyPressedEvent keyEvent = *dynamic_cast<KeyPressedEvent*>(e.get());
                std::shared_ptr<SingleplayerSelectGui> singleplayerGui = std::dynamic_pointer_cast<SingleplayerSelectGui>(game->getGui());
                singleplayerGui->handleKeyPressed(keyEvent.key);
            }
        }
    }

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

    if(e->getEventID() == "LEFT_CLICK") {
        if(game->getGui() != nullptr) {
            game->getGui()->mouseClick(mouseX, mouseY);
        }
    }

    if(e->getEventID() == "SCROLL") {
        ScrollEvent scrollEvent = *dynamic_cast<ScrollEvent*>(e.get());
        if(game->getGui() != nullptr) {
            game->getGui()->scrollHandle(scrollEvent.offsetX, scrollEvent.offsetY);
        }
    }
}