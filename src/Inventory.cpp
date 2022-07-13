#include "Inventory.h"

Inventory::Inventory(int slots) {
    items.resize(slots);
}

ItemStack& Inventory::getItemStackInSlot(int slot) {
    return items[slot];
}

void Inventory::setItemStackInSlot(int slot, ItemStack item) {
    items[slot] = item;
}

ItemStack Inventory::interactHeldItemAtSlot(ItemStack held, int slot) {
    if(held.getItem() == nullptr) {
        if(items[slot].getItem() == nullptr) {
            return held;
        }
    }
    
    if(canCombine(held, slot) > 0) {
        if(held.getItem() == nullptr) {
            if(items[slot].getItem() == nullptr) {
                return held;
            }
            held.setItem(items[slot].getItem());
        }else {
            items[slot].setItem(held.getItem());
        }

        int combine = canCombine(held, slot);

        if(!held.isEmpty()) {
            held.subtract(combine);
            items[slot].add(combine);
            return held;
        }else {
            held.add(combine);
            items[slot].subtract(combine);
            return held;
        }
    }else {
       ItemStack temp1 = held;
       ItemStack temp2 = items[slot];
       held = temp2;
       items[slot] = temp1;
       return held;
    }
}

int Inventory::canCombine(ItemStack held, int slot) {
    if(held.isEmpty() || items[slot].isEmpty()) {
        int heldCount = held.getCount();
        int slotCount = items[slot].getCount();
        std::shared_ptr<Item> item = held.getItem();

        int safeMaxStack = ((held.getItem() == nullptr) ? items[slot].getItem()->maxStack() : held.getItem()->maxStack());
        
        if(heldCount + slotCount <= safeMaxStack) {
            return heldCount;
        }else {
            return heldCount - ((heldCount + slotCount) - safeMaxStack);
        }
    }
    
    if(held.getItem()->getItemName() == items[slot].getItem()->getItemName()) {
        int heldCount = held.getCount();
        int slotCount = items[slot].getCount();
        std::shared_ptr<Item> item = held.getItem();

        if(heldCount + slotCount <= item->maxStack()) {
            return heldCount;
        }else {
            return heldCount - ((heldCount + slotCount) - item->maxStack());
        }
    }
    return 0;
}
