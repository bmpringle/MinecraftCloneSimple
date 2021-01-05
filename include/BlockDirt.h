#ifndef BLOCKDIRT_H
#define BLOCKDIRT_H

#include "Block.h"

class BlockDirt : public Block {
    public:
        BlockDirt();

        std::string getTextureName() override;      
};
#endif