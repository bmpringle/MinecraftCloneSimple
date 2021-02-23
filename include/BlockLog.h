#ifndef BLOCKLOG_H
#define BLOCKLOG_H

#include "Block.h"

class BlockLog : public Block {
    public:
        BlockLog();
        
        std::string getName() override;

        std::string getTextureName(SideEnum side) override;  
};
#endif