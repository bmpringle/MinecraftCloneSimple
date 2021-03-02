#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <Events.h>

class ItemStack;

class BlockPos;

class World;

class Item {
    public:
        virtual std::string getItemName();

        virtual std::string getIcon();

        virtual void onUse(World* world, ItemStack* stack);

        virtual void onLeftClick(World* world, BlockPos* blockLookingAt, ItemStack* stack);

        virtual int maxStack();

        virtual ~Item() = default;
    private:
};
#endif