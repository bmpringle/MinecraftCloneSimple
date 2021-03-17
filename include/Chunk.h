#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"
#include <vector>
#include <array>
#include "BinaryTree/BinaryTree.h"
#include "BlockData.h"
#include "SBDA.h"
#include <memory>

class Entity;

class Chunk {
    public:
        Chunk(int xLoc, int zLoc);

        Chunk(int xLoc, int zLoc, bool _isFakeChunk);

        BlockData getBlockAtLocation(BlockPos pos);

        BlockData& getBlockReferenceAtLocation(BlockPos pos);

        void setBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block);

        void setBlockDataAtLocation(BlockPos pos, BlockData data);

        void softSetBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block);

        void setColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void softSetColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void removeBlockAtLocation(BlockPos pos);

        void updateChunk(BlockArrayData* data);

        BlockPos getChunkCoordinates() const;

        AABB getChunkAABB();

        std::vector<BlockData> getBlocksInChunk();

        static std::array<int, 3> getChunkSize();

        bool isFakeChunk();

        BinaryTree<SBDA, AABB, SBDA>* getBlockTree();

        void addEntityAtPositionOfType(std::shared_ptr<Entity> entity, Pos pos);

        std::map<int, std::shared_ptr<Entity>>& getEntitiesInChunk();

    private:
        void initTree();

        static const int X = 16;
        static const int Y = 256;
        static const int Z = 16;
        BlockPos chunkCoordinates; 
        AABB chunkAABB;
        BinaryTree<SBDA, AABB, SBDA> blockTree;

        std::vector<BlockPos> blocksToUpdate = std::vector<BlockPos>();
        
        bool isFake = false;

        BlockData dummyData = BlockData();

        std::map<int, std::shared_ptr<Entity>> entityMap;

        //for communication purposes with the entities for movement
        int idCounter = 0;
};
#endif