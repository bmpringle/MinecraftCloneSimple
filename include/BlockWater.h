#ifndef BLOCKWATER_H
#define BLOCKWATER_H

#include "Block.h"

class BlockWater : public Block {
    public:
        BlockWater();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side, int data) override;  

        bool isSolid(int data) override;  

        bool isOpaque(int data) override;

        BlockRenderedModel getRenderedModel(int data) override;
};
#endif
