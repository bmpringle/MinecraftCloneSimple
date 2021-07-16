#ifndef BLOCKS_H
#define BLOCKS_H

#include "BlockDirt.h"
#include "BlockCobblestone.h"
#include "BlockGrass.h"
#include "BlockLog.h"
#include "BlockLeaf.h"
#include "BlockWater.h"
#include "BlockPlanks.h"
#include "BlockDoor.h"

#include "TextureArrayCreator.h"
#include "ModelRegister.h"

#include <map>

class Blocks {
    public: 
        static std::map<std::string, std::shared_ptr<Block>> blockMap;

        static const std::shared_ptr<Block> dirt;
        static const std::shared_ptr<Block> cobblestone;
        static const std::shared_ptr<Block> grass;
        static const std::shared_ptr<Block> log;
        static const std::shared_ptr<Block> leaf;
        static const std::shared_ptr<Block> water;
        static const std::shared_ptr<Block> planks;
        static const std::shared_ptr<Block> door;

        static void initTextureArrayCreator(TextureArrayCreator* texCreator);
        static void initModelRegister(ModelRegister* modelRegister, TextureArrayCreator* texCreator);
};
#endif