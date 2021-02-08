#include "ItemBlock.h"
#include "World.h"

ItemBlock::ItemBlock(std::shared_ptr<Block> block) : block(block){

}

std::string ItemBlock::getItemName() {
    return block->getName();
}

void ItemBlock::onRightClick(World* world, RightMouseButtonPressedEvent event) {
    if(world->getPlayer()->getBlockLookingAt() != nullptr) {
        BlockPos location = BlockPos(0, 0, 0);
        BlockPos* blockLookingAt = world->getPlayer()->getBlockLookingAt();
        int sideOfBlockLookingAt = world->getPlayer()->getSideOfBlockLookingAt();

        switch(sideOfBlockLookingAt) {
            default:
                break;
            case 1:
                location = BlockPos(blockLookingAt->x - 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                     
                }
                break;
            case 2:
                location = BlockPos(blockLookingAt->x + 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                      
                }
                break;
            case 3:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y - 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                      
                }
                break;
            case 4:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y + 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }
                }
                break;
            case 5:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z - 1);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                       
                }
                break;
            case 6:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z + 1);
                if(world->getBlockData()->getBlockAtPosition(location) == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block->create());

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                    
                }
                break;
        }
    }
}