#ifndef ITEMBLOCK_H
#define ITEMBLOCK_H

#include "Item.h"
#include "Block.h"
#include <memory>

class ItemBlock : public Item {
    public:
        ItemBlock(std::shared_ptr<Block> block);

        std::string getItemName() override;

        void onRightClick(World* world) override;

    private:
        std::shared_ptr<Block> block;
};
#endif