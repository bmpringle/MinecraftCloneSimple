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

        std::string getCountTextureID(Renderer* renderer);

    private:
        std::shared_ptr<Item> item = nullptr;

        int count = 0;

        static unsigned int itemStackCount;

        std::string textureID;

        int textureCount = -1;
};
#endif