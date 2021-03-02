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

        bool isAABBInWater(AABB playerAABB);

    private:
        std::vector<Chunk> chunkList;

        int size[3];
        bool updateRenderer = false;
        Chunk* fakeChunk = new Chunk(0, 0, true);

        std::map<BlockPos, LoadedChunkInfo> loadedChunkLocations = std::map<BlockPos,LoadedChunkInfo>();

        double SEED = abs(rand() % 10000);
        double zNoise = rand() % 1 - 0.5;

        double width = Chunk::getChunkSize()[0];
        double height = Chunk::getChunkSize()[2];

        int averageTerrainHeight = 10;
        double amplifier = 3;
        int waterLevel = 7;

        noise::module::Perlin noise = noise::module::Perlin();
};
#endif