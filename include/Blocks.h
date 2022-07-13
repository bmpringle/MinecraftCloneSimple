#ifndef BLOCKS_H
#define BLOCKS_H

#include <map>
#include "Block.h"

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
        static const std::shared_ptr<Block> glass;
};
#endif