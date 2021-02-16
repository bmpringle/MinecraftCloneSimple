#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include "Block.h"
#include "BlockPos.h"

class BlockData {
    public:
        BlockData(std::shared_ptr<Block> type, BlockPos pos);

        BlockData();

        BlockPos getPos();

        void setPos(BlockPos pos_);

        AABB getAABB();

        std::shared_ptr<Block> getBlockType();
    private:
        BlockPos pos;
        std::shared_ptr<Block> type;
};
#endif