#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H

#include "Block.h"

class BlockGrass : public Block {
    public:
        BlockGrass();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side) override;  
};
#endif