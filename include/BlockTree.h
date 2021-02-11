#ifndef BLOCKTREE_H
#define BLOCKTREE_H

#include <memory>
#include <vector>
#include "BlockPos.h"
#include "Block.h"
#include "BlockNode.h"

class BlockTree {
    public:
        BlockTree(int chunkSize);

        std::shared_ptr<Block> getBlockAtRelativeLocation(BlockPos pos);

        void setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block);

        void removeBlockAtRelativeLocation(BlockPos pos);

        std::vector<std::shared_ptr<Block>> getBlocksInChunk();

        bool doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block);
    private:
        BlockNode node;
};

#endif