#include "ItemBlock.h"
#include "World.h"

ItemBlock::ItemBlock(std::shared_ptr<Block> block) : block(block){

}

std::string ItemBlock::getItemName() {
    return block->getName();
}

void ItemBlock::onRightClick(World* world) {
    if(world->getPlayer()->getBlockLookingAt() != nullptr) {
        BlockPos location = BlockPos(0, 0, 0);
        BlockPos* blockLookingAt = world->getPlayer()->getBlockLookingAt();
        SideEnum sideOfBlockLookingAt = world->getPlayer()->getSideOfBlockLookingAt();

        switch(sideOfBlockLookingAt) {
            default:
                break;
            case UP:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y + 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                     
                }
                break;
            case DOWN:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y - 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                      
                }
                break;
            case NORTH:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z + 1);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                      
                }
                break;
            case SOUTH:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z - 1);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }
                }
                break;
            case EAST:
                location = BlockPos(blockLookingAt->x + 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                       
                }
                break;
            case WEST:
                location = BlockPos(blockLookingAt->x - 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr) {
                    world->getBlockData()->setBlockAtPosition(location, block);

                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), *world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                    }                    
                }
                break;
        }
    }
}