#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"
#include <memory>
#include "RenderInclude.h"
#include "Chunk.h"
#include "LoadedChunkInfo.h"

class World;

class BlockArrayData {
    public:
        BlockArrayData(int xSize, int ySize, int zSize);

        void updateBlockAtPosition(BlockPos pos);

        void setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        std::vector<Chunk> getRawChunkArray();

        std::shared_ptr<Block> getBlockAtPosition(BlockPos pos);

        void removeBlockAtPosition(BlockPos pos);

        Chunk* getChunkWithBlock(BlockPos pos);

        //checks if rendererupdate flag is set
        bool shouldUpdateRenderer();

        //tells object to reset rendererupdate flag
        void hasUpdatedRenderer();

        void updateLoadedChunks(BlockPos pos, World* world);

        const std::vector<LoadedChunkInfo> getLoadedChunkLocations();

        const std::vector<bool> getChunkLocationsToUpdate();

        bool isChunkLoaded(Chunk c);

        void setAllLoadedChunksToBeUpdated();

        bool isValidPosition(AABB aabb, float* yPos);

    private:
        std::vector<Chunk> rawBlockData;
        int size[3];
        bool updateRenderer = false;
        Chunk* fakeChunk = new Chunk(0, 0, true);

        std::vector<LoadedChunkInfo> loadedChunkLocations = std::vector<LoadedChunkInfo>();
};
#endif