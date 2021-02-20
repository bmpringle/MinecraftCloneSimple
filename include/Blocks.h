#include "BlockDirt.h"
#include "BlockCobblestone.h"
#include "BlockGrass.h"
#include "TextureArrayCreator.h"

#ifndef BLOCKS_H
#define BLOCKS_H

class Blocks {
    public: 

        const static std::shared_ptr<Block> dirt;
        const static std::shared_ptr<Block> cobblestone;
        const static std::shared_ptr<Block> grass;

        static void initTextureArrayCreator(TextureArrayCreator* texCreator);
};
#endif