#include "World.h"

void setup(World* world) {

}

int main() {
    World world = World();

    setup(&world);

    while(true) {
        world.updateGame();
        world.renderGame();
    }
}