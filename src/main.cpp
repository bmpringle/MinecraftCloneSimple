#include "Game.h"
#include <iostream>
#include "FontLoader.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    Game game = Game(nullptr);

    game.start();
}
