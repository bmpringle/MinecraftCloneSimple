#include "ItemStack.h"
#include <iostream>
#include "RenderInclude.h"
#include "World.h"

int ItemStack::itemStackCount = 0;

ItemStack::ItemStack(std::shared_ptr<Item> item, int count) : item(item), count(count) {
    textureID = "itemStackTexture" + std::to_string(itemStackCount);
    ++itemStackCount;
}

std::shared_ptr<Item> ItemStack::getItem() {
    return item;
}

int ItemStack::getCount() {
    return count;
}

void ItemStack::add(int count) {
    this->count += count;
}

void ItemStack::subtract(int count) {
    this->count -= count;
}

void ItemStack::setCount(int count) {
    this->count = count;
}

void ItemStack::setItem(std::shared_ptr<Item> item) {
    this->item = item;
}

bool ItemStack::isEmpty() {
    return (count == 0) || (item == nullptr);
}

void ItemStack::onUse(World* world) {
    if(item != nullptr) {
        item->onRightClick(world, this);
    }else {
        if(world->getPlayer()->getBlockLookingAt() != nullptr) {
            BlockPos chunkLocation = world->getBlockData()->getChunkWithBlock(*world->getPlayer()->getBlockLookingAt())->getChunkCoordinates();
            world->getBlockData()->setChunkToUpdate(chunkLocation);
            if(!world->getPlayer()->isEntitySneaking()) {
                if(world->getBlockData()->getBlockReferenceAtPosition(*world->getPlayer()->getBlockLookingAt()).activateBlock(world, this)) {
                    return;
                }
            }
        }
    }
}

void ItemStack::onLeftClick(World* world, BlockPos* posLookingAt) {
    if(item != nullptr) {
        item->onLeftClick(world, posLookingAt, this);
    }
}

ItemStack::ItemStack() {

}

std::string ItemStack::getCountTextureID(Renderer* renderer) {
    if(textureCount != count) {
        textureCount = count;
        renderer->textTextureBuffer(textureID, std::to_string(count));
    }
    return textureID;
}
