#include "ItemStack.h"
#include <iostream>
#include "RenderInclude.h"

ItemStack::ItemStack(std::shared_ptr<Item> item, int count) : item(item), count(count) {

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
        item->onUse(world, this);
    }
}

void ItemStack::onLeftClick(World* world, BlockPos* posLookingAt) {
    if(item != nullptr) {
        item->onLeftClick(world, posLookingAt, this);
    }
}

ItemStack::ItemStack() {

}

unsigned int ItemStack::getCountTBO(WorldRenderer* renderer) {
    if(tboCount != count) {
        tboCount = count;
        glDeleteTextures(1, &TBO);
        TBO = renderer->textTextureBuffer(std::to_string(count));
        return TBO;
    }else {
        return TBO;
    }
}