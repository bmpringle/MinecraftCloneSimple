#ifndef GAMEEVENTHANDLER_H
#define GAMEEVENTHANDLER_H

#include "World.h"

class Game;

class GameEventHandler final : public Listener {
    public:
        GameEventHandler(Game* game);

        void listenTo(std::shared_ptr<Event> e);
    private:
        Game* game;
        int mouseX = 0;
        int mouseY = 0;
};
#endif