#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <array>

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        std::shared_ptr<Block> getBlockAtRelativeLocation(BlockPos pos);

        void setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block);

        BlockPos getChunkCoordinates();

        AABB getChunkAABB();

        std::array<std::array<std::array<std::shared_ptr<Block>, 10>, 100>, 10> getBlocksInChunk();

    private:
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        std::array<std::array<std::array<std::shared_ptr<Block>, 10>, 100>, 10> blocksInChunk;
};
#endif