#include "World.h"
#include "Player.h"

World::World() : input(InputHandler()), renderer(WorldRenderer()), thePlayer(Player()), worldEventQueue(EventQueue()), internalBlockData(BlockArrayData(100, 100, 100)) {
    generateWorld();
}

void World::updateGame() {
    input.handleInput(&worldEventQueue);
    for(int x = 0; x < 100; ++x) {
        for(int y = 0; y < 100; ++y) {
            for(int z = 0; z < 100; ++z) {
                internalBlockData.updateBlockAtPosition(BlockPos(x, y, z));
            }
        }
    }
    thePlayer.updatePlayerInWorld(this);
}

void World::generateWorld() {
     
}

void World::renderGame() {

}