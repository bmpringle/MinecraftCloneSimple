#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"
#include <memory>
#include "Chunk.h"
#include "LoadedChunkInfo.h"
#include <mutex>
#include <map>
#include <noise/noise.h>

class World;

class BlockArrayData {
    public:
        BlockArrayData(std::string worldFolder_, int seed);

        void sendChunkUpdates();

        void setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        void setBlockDataAtPosition(BlockPos pos, BlockData data);

        void softSetBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        void setColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void softSetColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount);

        void setWorldFolder(std::string path);

        std::vector<Chunk>& getChunkArrayReference();

        BlockData getBlockAtPosition(BlockPos pos);

        BlockData& getBlockReferenceAtPosition(BlockPos pos);

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

        void loadChunkFromFile(std::string chunkPath, BlockPos chunkLocation);

        void unloadChunkToFile(BlockPos chunkLocation, std::string worldname);

        int getSeed();

        void setChunkToUpdate(BlockPos chunkLocation);

        //should be called every logic tick, syncronizes chunks constructed by other threads.
        void updateChunkList();

    private:
        void loadChunksFromFileAsync(std::vector<std::string> chunkPaths, std::vector<BlockPos> chunkLocations);

        void unloadChunksToFileAsync(std::vector<BlockPos> chunkLocations, std::string worldName);
        
        std::vector<Chunk> chunkList;

        bool updateRenderer = false;
        Chunk* fakeChunk = new Chunk(0, 0, true);

        std::map<BlockPos, LoadedChunkInfo> loadedChunkLocations = std::map<BlockPos,LoadedChunkInfo>();

        int SEED;
        double zNoise = rand() % 1 - 0.5;

        double width = Chunk::getChunkSize().at(0);
        double height = Chunk::getChunkSize().at(2);

        int averageTerrainHeight = 30;
        double amplifier = 3;
        int waterLevel = 28;

        noise::module::Perlin noise = noise::module::Perlin();

        std::string worldFolder = "";

        BlockData dummyData = BlockData();

        std::vector<Chunk> threadSafeChunkList;
        std::mutex threadSafeChunkMutex;
};
#endif
