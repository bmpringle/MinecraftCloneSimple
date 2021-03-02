#include "Item.h"
#include "World.h"
#include "Block.h"
#include "ItemStack.h"
#include "ItemStack.h"

void Item::onUse(World* world, ItemStack* stack) {

}

void Item::onLeftClick(World* world, BlockPos* blockLookingAt, ItemStack* stack) {

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