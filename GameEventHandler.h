#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include "World.h"

class Game;

class GameEventHandler : public Listener {
    public:
        GameEventHandler(Game* game);

        void listenTo(std::shared_ptr<Event> e);
    private:
        Game* game;
};
#endif