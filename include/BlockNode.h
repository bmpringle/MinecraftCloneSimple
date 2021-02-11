#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <memory>
#include "Block.h"
#include "BlockPos.h"

class BlockNode;

class BlockNode {
    public:
        BlockNode(BlockPos pos, int xSize, int zSize, bool recursiveFlag);

        std::shared_ptr<Block> getBlockAtRelativeLocation(BlockPos pos);

        void setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block);

        void removeBlockAtRelativeLocation(BlockPos pos);

        void getBlocksInChunk(std::vector<std::shared_ptr<Block>>* blocks);

        bool doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block);

        BlockPos getNodeLocation();

        AABB getAABB();
    private:
        std::unique_ptr<BlockNode> left = nullptr;
        std::unique_ptr<BlockNode> right = nullptr;
        
        BlockPos chunkLocation;

        std::shared_ptr<Block> block = nullptr;
        int xSize = 0;
        int zSize = 0;
};
#endif