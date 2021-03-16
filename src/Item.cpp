#include "Item.h"
#include "World.h"
#include "Block.h"
#include "ItemStack.h"
#include "ItemStack.h"

void Item::onUse(World* world, ItemStack* stack) {

}

void Item::onLeftClick(World* world, BlockPos* blockLookingAt, ItemStack* stack) {

}

void Item::onRightClick(World* world, ItemStack* stack) {
    if(world->getPlayer()->getBlockLookingAt() != nullptr) {
        BlockPos chunkLocation = world->getBlockData()->getChunkWithBlock(*world->getPlayer()->getBlockLookingAt())->getChunkCoordinates();
        world->getBlockData()->setChunkToUpdate(chunkLocation);
        if(!world->getPlayer()->isEntitySneaking()) {
            if(world->getBlockData()->getBlockReferenceAtPosition(*world->getPlayer()->getBlockLookingAt()).activateBlock(world, stack)) {
                return;
            }
        }
    }
    onUse(world, stack);
}

std::string Item::getItemName() {
    return "item";
}

std::string Item::getIcon() {
    return "item.png";
}

int Item::maxStack() {
    return 64;
}
