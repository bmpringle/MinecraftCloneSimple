#include "ItemBlock.h"
#include "World.h"

ItemBlock::ItemBlock(std::shared_ptr<Block> block) : block(block){

}

std::string ItemBlock::getItemName() {
    return block->getName();
}

void ItemBlock::onUse(World* world, ItemStack* stack) {
    if(world->getPlayer()->getBlockLookingAt() != nullptr && stack->getCount() > 0 && stack->getItem() != nullptr) {
        BlockPos location = BlockPos(0, 0, 0);
        BlockPos* blockLookingAt = world->getPlayer()->getBlockLookingAt();
        SideEnum sideOfBlockLookingAt = world->getPlayer()->getSideOfBlockLookingAt();

        switch(sideOfBlockLookingAt) {
            default:
                break;
            case UP:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y + 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }                  
                }
                break;
            case DOWN:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y - 1, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }                       
                }
                break;
            case NORTH:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z + 1);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }                       
                }
                break;
            case SOUTH:
                location = BlockPos(blockLookingAt->x, blockLookingAt->y, blockLookingAt->z - 1);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }    
                }
                break;
            case EAST:
                location = BlockPos(blockLookingAt->x + 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }                    
                }
                break;
            case WEST:
                location = BlockPos(blockLookingAt->x - 1, blockLookingAt->y, blockLookingAt->z);
                if(world->getBlockData()->getBlockAtPosition(location).getBlockType() == nullptr || !world->getBlockData()->getBlockAtPosition(location).isSolid()) {
                    BlockData oldBlock = world->getBlockData()->getBlockAtPosition(location);
                    world->getBlockData()->setBlockAtPosition(location, block);
                    world->getBlockData()->getBlockReferenceAtPosition(location).placedOnSide(world->getPlayer()->horizontalSidePlacedOn(), world->getPlayer()->sideLookingAt());
                    stack->subtract(1);
                    bool valid = world->getPlayer()->validatePosition(world->getPlayer()->getPos(), world->getBlockData());
                    if(!valid) {
                        world->getBlockData()->removeBlockAtPosition(location);
                        if(oldBlock.getBlockType() != nullptr) {
                            world->getBlockData()->setBlockDataAtPosition(location, oldBlock);
                        }
                        stack->add(1);
                    }
                break;
            }
        }
    }
}

std::string ItemBlock::getIcon() {
    return block->getTextureName(NORTH);
}