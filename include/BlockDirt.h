#ifndef BLOCKDIRT_H
#define BLOCKDIRT_H

#include "Block.h"

class BlockDirt : public Block {
    public:
        BlockDirt();

        std::string getName() override;

        std::string getTextureName() override;   

        std::shared_ptr<Block> create() override;   
};
#endif