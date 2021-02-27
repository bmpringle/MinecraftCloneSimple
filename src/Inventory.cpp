#include "Inventory.h"

Inventory::Inventory(int slots) {
    items.resize(slots);
}

ItemStack Inventory::getItemStackInSlot(int slot) {
    return items[slot];
}

void Inventory::setItemStackInSlot(int slot, ItemStack item) {
    items[slot] = item;
}