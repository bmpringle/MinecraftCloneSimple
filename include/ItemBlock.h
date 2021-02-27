#ifndef ITEMBLOCK_H
#define ITEMBLOCK_H

#include "Item.h"
#include "Block.h"
#include <memory>
#include "ItemStack.h"

class ItemBlock : public Item {
    public:
        ItemBlock(std::shared_ptr<Block> block);

        std::string getItemName() override;

        void onUse(World* world, ItemStack* stack) override;

        std::string getIcon() override;

    private:
        std::shared_ptr<Block> block;
};
#endif