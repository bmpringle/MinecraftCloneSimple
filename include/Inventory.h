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

        ItemStack& operator[](int index) {
            return items[index];
        }

    private:
        std::vector<ItemStack> items = std::vector<ItemStack>();
};
#endif