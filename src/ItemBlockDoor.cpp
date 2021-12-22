#include "ItemBlockDoor.h"
#include "World.h"

ItemBlockDoor::ItemBlockDoor() {

}

std::string ItemBlockDoor::getItemName() {
    return Blocks::door->getName();
}

void ItemBlockDoor::onUse(World* world, ItemStack* stack) {
    if(world->getPlayer()->getBlockLookingAt() != nullptr && stack->getCount() > 0 && stack->getItem() != nullptr) {
        BlockPos location = BlockPos(0, 0, 0);
        BlockPos* blockLookingAt = world->getPlayer()->getBlockLookingAt();
        SideEnum sideOfBlockLookingAt = world->getPlayer()->getSideOfBlockLookingAt();

        switch(sideOfBlockLookingAt) {
            default:
                break;
            case UP:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y + 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).isBlockAir() || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    //check if block below is solid
                    if(!world->getBlockData()->getBlockAtPosition(*blockLookingAt).isBlockAir() && world->getBlockData()->getBlockAtPosition(*blockLookingAt).isSolid()) {
                        //check if block above doesn't exist
                        BlockPos secondDoorBlock = BlockPos(location.x, location.y + 1, location.z);
                        if(world->getBlockData()->getBlockAtPosition(secondDoorBlock).isBlockAir() || !world->getBlockData()->getBlockAtPosition(secondDoorBlock).isSolid()) {
                            BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                            world->getBlockData()->setBlockAtPosition(location, Blocks::door);
                            world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                            BlockData secondBlockOld = world->getBlockData()->getBlockAtPosition(secondDoorBlock);
                            BlockData secondBlock = BlockData(Blocks::door, secondDoorBlock);
                            secondBlock.setData(1);
                            world->getBlockData()->setBlockDataAtPosition(secondDoorBlock, secondBlock);
                            world->getBlockData()->getBlockReferenceAtPosition(secondDoorBlock).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                            stack->subtract(1);
                            bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                            if(!valid) {
                                world->getBlockData()->removeBlockAtPosition(location);
                                if(!oldBlock.isBlockAir()) {
                                    world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                                }

                                world->getBlockData()->removeBlockAtPosition(secondDoorBlock);
                                if(!secondBlockOld.isBlockAir()) {
                                    world->getBlockData()->setBlockDataAtPosition(secondDoorBlock, secondBlockOld);
                                }
                                stack->add(1);
                            }
                        }
                    }                  
                }
                break;
        }
    }
}

std::string ItemBlockDoor::getIcon() {
    return Blocks::door->getTextureName(NORTH);
}