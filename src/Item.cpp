#include "Item.h"
#include "World.h"
#include "Block.h"

void Item::onRightClick(World* world, RightMouseButtonPressedEvent event) {

}

void Item::onLeftClick(World* world, LeftMouseButtonPressedEvent event, BlockPos* blockLookingAt) {
    if(blockLookingAt != nullptr) {
        BlockPos selected = *blockLookingAt;
        world->getBlockData()->removeBlockAtPosition(selected);
    }
}

std::string Item::getItemName() {
    return "item";
}
