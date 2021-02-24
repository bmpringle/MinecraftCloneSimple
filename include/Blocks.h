#include "BlockDirt.h"
#include "BlockCobblestone.h"
#include "BlockGrass.h"
#include "BlockLog.h"
#include "BlockLeaf.h"

#include "TextureArrayCreator.h"

#ifndef BLOCKS_H
#define BLOCKS_H

class Blocks {
    public: 

        const static std::shared_ptr<Block> dirt;
        const static std::shared_ptr<Block> cobblestone;
        const static std::shared_ptr<Block> grass;
        const static std::shared_ptr<Block> log;
        const static std::shared_ptr<Block> leaf;

        static void initTextureArrayCreator(TextureArrayCreator* texCreator);
};
#endif