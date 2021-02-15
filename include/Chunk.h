#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <array>
#include "BinaryTree/BinaryTree.h"

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        Chunk(int xLoc, int zLoc, bool _isFakeChunk);

        std::shared_ptr<Block> getBlockAtLocation(BlockPos pos);

        void setBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block);

        void removeBlockAtLocation(BlockPos pos);

        BlockPos getChunkCoordinates() const;

        AABB getChunkAABB();

        std::vector<std::shared_ptr<Block>> getBlocksInChunk();

        static std::array<int, 3> getChunkSize();

        bool isFakeChunk();

        BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>>* getBlockTree();

    private:
        bool doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block);
        void initTree();

        static const int X = 16;
        static const int Y = 256;
        static const int Z = 16;
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>> blockTree;
        
        bool isFake = false;
};
#endif