#ifndef BLOCKCOBBLESTONE_H
#define BLOCKCOBBLESTONE_H

#include "Block.h"

class BlockCobblestone : public Block {
    public:
        BlockCobblestone();
        
        std::string getName() override;

        std::string getTextureName() override;  

        std::shared_ptr<Block> create() override;    
};
#endif