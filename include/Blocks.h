#include "BlockDirt.h"
#include "BlockCobblestone.h"

#ifndef BLOCKS_H
#define BLOCKS_H

static std::shared_ptr<Block> dirt = std::make_shared<BlockDirt>();

static std::shared_ptr<Block> cobblestone = std::make_shared<BlockCobblestone>();

#endif