#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include "Item.h"
#include "ItemStack.h"

class Inventory {
    public: 
        Inventory(int slots);

        ItemStack getItemStackInSlot(int slot);

        void setItemStackInSlot(int slot, ItemStack item);

        //returns new held item
        ItemStack interactHeldItemAtSlot(ItemStack held, int slot);

        ItemStack& operator[](int index) {
            if(index < 0 || index >= items.size()) {
                throw std::out_of_range("bad");
            }
            return items[index];
        }

    private:
        int canCombine(ItemStack held, int slot);
        
        std::vector<ItemStack> items = std::vector<ItemStack>();
};
#endif