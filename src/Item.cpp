#include "Item.h"
#include "World.h"
#include "Block.h"

void Item::onRightClick(World* world) {

}

void Item::onLeftClick(World* world, BlockPos* blockLookingAt) {
    if(blockLookingAt != nullptr) {
        BlockPos selected = *blockLookingAt;
        world->getBlockData()->removeBlockAtPosition(selected);
    }
}

std::string Item::getItemName() {
    return "item";
}
