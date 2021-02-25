#ifndef BLOCKWATER_H
#define BLOCKWATER_H

#include "Block.h"

class BlockWater : public Block {
    public:
        BlockWater();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side) override;  

        bool isSolid() override;  

        bool isOpaque() override;
};
#endif