#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "Item.h"
#include <memory>

class ItemStack {
    public:
        ItemStack();

        ItemStack(std::shared_ptr<Item> item, int count);

        std::shared_ptr<Item> getItem();

        int getCount();

        void setCount(int count);

        void setItem(std::shared_ptr<Item> item);

        void add(int count);

        void subtract(int count);

        bool isEmpty();

        void onUse(World* world);

        void onLeftClick(World* world, BlockPos* posLookingAt);

    private:
        std::shared_ptr<Item> item = nullptr;
        int count = 0;
};
#endif