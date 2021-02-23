#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"
#include <memory>
#include "RenderInclude.h"
#include "Chunk.h"
#include "LoadedChunkInfo.h"
#include <mutex>
#include <map>
#include <noise/noise.h>

class World;

class BlockArrayData {
    public:
        BlockArrayData(int xSize, int ySize, int zSize);

        void updateBlockAtPosition(BlockPos pos);

        void setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        void softSetBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        void setColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void softSetColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        std::vector<Chunk> getRawChunkArray();

        BlockData getBlockAtPosition(BlockPos pos);

        void removeBlockAtPosition(BlockPos pos);

        Chunk* getChunkWithBlock(BlockPos pos);

        //checks if rendererupdate flag is set
        bool shouldUpdateRenderer();

        //tells object to reset rendererupdate flag
        void hasUpdatedRenderer();

        void updateLoadedChunks(BlockPos pos, World* world);

        const std::map<BlockPos, LoadedChunkInfo> getLoadedChunkLocations();

        const std::vector<bool> getChunkLocationsToUpdate();

        bool isChunkLoaded(Chunk c);

        void setAllLoadedChunksToBeUpdated();

        bool isValidPosition(AABB aabb, float* yPos);

        void generateChunk(BlockPos chunkLocation);

    private:
        std::vector<Chunk> chunkList;

        int size[3];
        bool updateRenderer = false;
        Chunk* fakeChunk = new Chunk(0, 0, true);

        std::map<BlockPos, LoadedChunkInfo> loadedChunkLocations = std::map<BlockPos,LoadedChunkInfo>();

        const double SEED = abs(rand() % 10000);
        double zNoise = rand() % 1 - 0.5;

        const double width = Chunk::getChunkSize()[0];
        const double height = Chunk::getChunkSize()[2];

        const int averageTerrainHeight = 10;
        const double amplifier = 3;

        noise::module::Perlin noise;
};
#endif