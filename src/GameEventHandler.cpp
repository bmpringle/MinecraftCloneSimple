#include "GameEventHandler.h"
#include "Events.h"
#include "Game.h"

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