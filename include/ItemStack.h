#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "Item.h"
#include <memory>
#include "WorldRenderer.h"

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

        unsigned int getCountTBO(WorldRenderer* renderer);

    private:
        std::shared_ptr<Item> item = nullptr;
        int count = 0;

        unsigned int TBO = -1;
        int tboCount = -1;
};
#endif