#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <array>
#include "BinaryTree/BinaryTree.h"
#include "BlockData.h"

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        Chunk(int xLoc, int zLoc, bool _isFakeChunk);

        BlockData getBlockAtLocation(BlockPos pos);

        void setBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block);

        void softSetBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block);

        void setColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void softSetColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void removeBlockAtLocation(BlockPos pos);

        BlockPos getChunkCoordinates() const;

        AABB getChunkAABB();

        std::vector<BlockData> getBlocksInChunk();

        static std::array<int, 3> getChunkSize();

        bool isFakeChunk();

        BinaryTree<std::array<BlockData, 256>, AABB, std::array<BlockData, 256>>* const getBlockTree();

    private:
        void initTree();

        static const int X = 16;
        static const int Y = 256;
        static const int Z = 16;
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        BinaryTree<std::array<BlockData, 256>, AABB, std::array<BlockData, 256>> blockTree;

        std::vector<BlockPos> blocksToUpdate = std::vector<BlockPos>();
        
        bool isFake = false;
};
#endif