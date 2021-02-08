#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <array>

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        Chunk(int xLoc, int zLoc, bool _isFakeChunk);

        std::shared_ptr<Block> getBlockAtRelativeLocation(BlockPos pos);

        void setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block);

        void removeBlockAtRelativeLocation(BlockPos pos);

        BlockPos getChunkCoordinates() const;

        AABB getChunkAABB();

        std::vector<std::shared_ptr<Block>> getBlocksInChunk();

        static std::array<int, 3> getChunkSize();

        bool isFakeChunk();

    private:
        bool doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block);

        static const int X = 16;
        static const int Y = 256;
        static const int Z = 16;
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        std::vector<std::shared_ptr<Block>> blocksInChunk;

        bool isFake = false;
};
#endif