#ifndef BLOCKLEAF_H
#define BLOCKLEAF_H

#include "Block.h"

class BlockLeaf : public Block {
    public:
        BlockLeaf();

        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;   
};
#endif