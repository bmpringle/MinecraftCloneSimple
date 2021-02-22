#include "BlockArrayData.h"
#include <iostream>
#include <math.h>
#include "World.h"
#include <algorithm>
#include <Blocks.h>
#include <noise/noise.h>

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize) : chunkList(std::vector<Chunk>()) {
    size[0] = xSize; 
    size[1] = ySize; 
    size[2] = zSize;
}

void BlockArrayData::updateBlockAtPosition(BlockPos pos) {

}

void BlockArrayData::updateLoadedChunks(BlockPos pos, World* world) {   

    Chunk* centerChunk = getChunkWithBlock(pos);

    int renderDistance = world->getChunkRenderDistance();
    std::array<int, 3> size = Chunk::getChunkSize();

    std::map<BlockPos, LoadedChunkInfo> oldChunks = loadedChunkLocations;

    loadedChunkLocations.clear();
    //auto start = std::chrono::high_resolution_clock::now();
    for(float x = -(float)renderDistance; x <= (float)renderDistance; ++x) {
        for(float z = -(float)renderDistance; z <= (float)renderDistance; ++z) {
            BlockPos playerBlock = BlockPos((int)centerChunk->getChunkCoordinates().x + size[0] * x, 0, (int)centerChunk->getChunkCoordinates().z + size[2] * z);
            if(!getChunkWithBlock(playerBlock)->isFakeChunk()) {
                BlockPos chunkLocation = getChunkWithBlock(playerBlock)->getChunkCoordinates();
                LoadedChunkInfo info = LoadedChunkInfo(BlockPos(0, 0, 0), false);
                try {
                    info = oldChunks.at(chunkLocation);
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, info.update);
                }catch(std::out_of_range e) {
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, true);
                }
            }else {
                generateChunk(playerBlock);
                LoadedChunkInfo info = LoadedChunkInfo(BlockPos(0, 0, 0), false);
                BlockPos chunkLocation = getChunkWithBlock(playerBlock)->getChunkCoordinates();
                try {
                    info = oldChunks.at(chunkLocation);       
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, info.update);
                }catch(std::out_of_range e) {
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, true);
                }
            }
        }
    }
    //std::cout << "duration1: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
}

void BlockArrayData::setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block) {

    std::array<int, 3> size = Chunk::getChunkSize();

    
    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).setBlockAtLocation(pos, block);
                    if(isChunkLoaded(c)) {
                        loadedChunkLocations[c.getChunkCoordinates()].update = true;
                    }
                    return;
                }
            }        
        }
    }
    
    Chunk c = Chunk(floor((float)pos.x / (float)size[0]), floor((float)pos.z / (float)size[2]));
    c.setBlockAtLocation(pos, block);
    chunkList.push_back(c);
}

bool BlockArrayData::isChunkLoaded(Chunk c) {
    try {
        loadedChunkLocations.at(c.getChunkCoordinates());
        return true;
    }catch(std::out_of_range e) {
        return false;
    }
}

void BlockArrayData::removeBlockAtPosition(BlockPos pos) {

    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
    
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).removeBlockAtLocation(pos);
                    if(isChunkLoaded(c)) {
                        loadedChunkLocations[c.getChunkCoordinates()].update = true;
                    }
                }
            }
        }
    }
}

//do not modify
BlockData BlockArrayData::getBlockAtPosition(BlockPos pos) {

    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
        
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    return chunkList.at(i).getBlockAtLocation(pos);
                }
            }
        }
    }
    return BlockData();
}

/*
 * do not modify. actually, just don't use. 
 * make another member function for whatever 
 * purpose you need, copying all this data is 
 * just not a fun time. ever.
 */
std::vector<Chunk> BlockArrayData::getRawChunkArray() {
    return chunkList;
}

//do not modify
Chunk* BlockArrayData::getChunkWithBlock(BlockPos pos) {
    
    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < chunkList.size(); ++i) {
        BlockPos location = chunkList[i].getChunkCoordinates();
        BlockPos locationEnd = chunkList[i].getChunkCoordinates() + BlockPos(size[0], size[1], size[2]);
        
        if(location.x <= pos.x && locationEnd.x > pos.x) {
            if(location.y <= pos.y && locationEnd.y > pos.y) {
                if(location.z <= pos.z && locationEnd.z > pos.z) {
                    return &chunkList[i];
                }
            }
        }
    }

    return fakeChunk;
}

bool BlockArrayData::shouldUpdateRenderer() {
    for(std::pair<BlockPos, LoadedChunkInfo> pair : loadedChunkLocations) {
        if(loadedChunkLocations[pair.first].update) {
            return true;
        }
    }
    return false;
}

void BlockArrayData::hasUpdatedRenderer() {
    for(std::pair<BlockPos, LoadedChunkInfo> pair : loadedChunkLocations) {
        loadedChunkLocations[pair.first].update = false;
    }
}

const std::map<BlockPos, LoadedChunkInfo> BlockArrayData::getLoadedChunkLocations() {
    return loadedChunkLocations;
}

//used when world is loaded to force a rendering update
void BlockArrayData::setAllLoadedChunksToBeUpdated() {
    for(std::pair<BlockPos, LoadedChunkInfo> pair : loadedChunkLocations) {
        loadedChunkLocations[pair.first].update = true;
    }
}

bool BlockArrayData::isValidPosition(AABB playerAABB, float* ypos) {
    for(std::pair<BlockPos, LoadedChunkInfo> pair : loadedChunkLocations) {
        Chunk* c = getChunkWithBlock(pair.first);
        if(!c->isFakeChunk()) {
            AABB chunkAABB = c->getChunkAABB();
            if(AABBIntersectedByAABB(playerAABB, chunkAABB)) {
                auto tree = c->getBlockTree();
                std::function<bool(AABB, bool, std::optional<std::array<BlockData, 256>>)> eval = [playerAABB](AABB aabb, bool isLeaf, std::optional<std::array<BlockData, 256>> block) -> bool { 
                    if(AABBIntersectedByAABB(playerAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<std::array<BlockData, 256>>*> blocksVector = tree->getLeafOfTree(eval);
                if(blocksVector.size() > 0) {
                    for(std::optional<std::array<BlockData, 256>>* blocksColumn : blocksVector) {
                        for(int i = 0; i < 256; ++i) {
                            BlockData block = (*blocksColumn).value()[i];
                            if(block.getBlockType() != nullptr) {
                                AABB blockAABB = block.getAABB();
                                if(AABBIntersectedByAABB(playerAABB, blockAABB)) {
                                    *ypos = blockAABB.startY + blockAABB.ySize;
                                    return false;
                                }
                            }
                        }
                    }
                }         
            }
        }
    }
    return true;
}

void BlockArrayData::generateChunk(BlockPos chunkLocation) {
    Chunk generatingChunk = Chunk(floor((float)chunkLocation.x / (float)Chunk::getChunkSize()[0]), floor((float)chunkLocation.z / (float)Chunk::getChunkSize()[2]));

    const double width = Chunk::getChunkSize()[0];
    const double height = Chunk::getChunkSize()[2];

    const int averageTerrainHeight = 10;
    const double amplifier = 3;

    noise::module::Perlin noise;
    noise.SetSeed(SEED);
    
    for(int x = chunkLocation.x; x < chunkLocation.x + width; ++x) {
        for(int z = chunkLocation.z; z < chunkLocation.z + height; ++z) {
            double nx = x/width - 0.5, ny = z/height - 0.5;
            double value = 1 * noise.GetValue(1 * nx, 1 * ny, zNoise) + 0.5 * noise.GetValue(2 * nx, 2 * ny, zNoise) + 0.25 * noise.GetValue(4 * nx, 4 * ny, zNoise) + 0.125 * noise.GetValue(8 * nx, 8 * ny, zNoise) + 0.0625 * noise.GetValue(16 * nx, 16 * ny, zNoise);

            int blockHeight = floor(value * amplifier) + averageTerrainHeight;
            for(int y = 0; y < blockHeight; ++y) {
                if(y + 1 == blockHeight) {
                    generatingChunk.setBlockAtLocation(BlockPos(x, y, z), Blocks::grass);
                }else if(y + 4 >= blockHeight) {
                    generatingChunk.setBlockAtLocation(BlockPos(x, y, z), Blocks::dirt);
                }else {
                    generatingChunk.setBlockAtLocation(BlockPos(x, y, z), Blocks::cobblestone);
                }
            }
        }
    }
    chunkList.push_back(generatingChunk);
}