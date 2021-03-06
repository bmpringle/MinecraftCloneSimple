#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "Item.h"
#include <memory>
#include "Renderer.h"

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

        unsigned int getCountTBO(Renderer* renderer);

    private:
        std::shared_ptr<Item> item = nullptr;
        int count = 0;

        unsigned int TBO = 0;
        bool TBOinit = false;

        int tboCount = -1;
};
#endif