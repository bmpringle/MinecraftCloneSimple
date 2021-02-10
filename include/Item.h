#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <Events.h>

class BlockPos;

class World;

class Item {
    public:
        virtual std::string getItemName();

        virtual void onRightClick(World* world);

        virtual void onLeftClick(World* world, BlockPos* blockLookingAt);

        virtual ~Item() = default;
    private:
};
#endif