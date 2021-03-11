#ifndef BLOCKPLANKS_H
#define BLOCKPLANKS_H

#include "Block.h"

class BlockPlanks : public Block {
    public:
        BlockPlanks();

        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;   
};
#endif