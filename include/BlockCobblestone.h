#ifndef BLOCKCOBBLESTONE_H
#define BLOCKCOBBLESTONE_H

#include "Block.h"

class BlockCobblestone : public Block {
    public:
        BlockCobblestone();

        std::string getTextureName() override;      
};
#endif