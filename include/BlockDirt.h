#ifndef BLOCKDIRT_H
#define BLOCKDIRT_H

#include "Block.h"

class BlockDirt : public Block {
    public:
        BlockDirt();

        std::string getName() override;

        std::string getTextureName(SideEnum side) override;   
};
#endif