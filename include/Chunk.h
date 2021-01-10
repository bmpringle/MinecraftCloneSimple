#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <array>

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        std::shared_ptr<Block> getBlockAtRelativeLocation(BlockPos pos);

        void setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block);

        void removeBlockAtRelativeLocation(BlockPos pos);

        BlockPos getChunkCoordinates();

        AABB getChunkAABB();

        std::vector<std::shared_ptr<Block>> getBlocksInChunk();

        static std::array<int, 3> getChunkSize();

    private:
        bool doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block);

        static const int X = 10;
        static const int Y = 100;
        static const int Z = 10;
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        std::vector<std::shared_ptr<Block>> blocksInChunk;
};
#endif