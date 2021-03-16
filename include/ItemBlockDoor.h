#ifndef ITEMBLOCKDOOR_H
#define ITEMBLOCKDOOR_H

#include "Item.h"
#include "Block.h"
#include <memory>
#include "ItemStack.h"

class ItemBlockDoor : public Item {
    public:
        ItemBlockDoor();

        std::string getItemName() override;

        void onUse(World* world, ItemStack* stack) override;

        std::string getIcon() override;
};
#endif