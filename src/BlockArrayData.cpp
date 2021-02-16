#include "BlockArrayData.h"
#include <iostream>
#include <math.h>
#include "World.h"
#include <algorithm>

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize) : rawBlockData(std::vector<Chunk>()) {
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

    std::vector<LoadedChunkInfo> oldChunks = loadedChunkLocations;

    loadedChunkLocations.clear();

    for(float x = -(float)renderDistance; x <= (float)renderDistance; ++x) {
        for(float z = -(float)renderDistance; z <= (float)renderDistance; ++z) {
            BlockPos playerBlock = BlockPos((int)centerChunk->getChunkCoordinates().x + size[0] * x, 0, (int)centerChunk->getChunkCoordinates().z + size[2] * z);
            if(!getChunkWithBlock(playerBlock)->isFakeChunk()) {
                BlockPos chunkLocation = getChunkWithBlock(playerBlock)->getChunkCoordinates();
                std::vector<LoadedChunkInfo>::iterator it = std::find_if(oldChunks.begin(), oldChunks.end(), [chunkLocation](LoadedChunkInfo l) {
                            return l.chunkLocation == chunkLocation;
                });
                loadedChunkLocations.push_back(LoadedChunkInfo(chunkLocation, (it == oldChunks.end()) ? true : oldChunks.at(it - oldChunks.begin()).update));
            }
        }
    }
}

void BlockArrayData::setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block) {

    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    rawBlockData.at(i).setBlockAtLocation(pos, block);
                    if(isChunkLoaded(c)) {
                        std::vector<LoadedChunkInfo>::iterator chunkLocIt = std::find_if(loadedChunkLocations.begin(), loadedChunkLocations.end(), [c](LoadedChunkInfo l) {
                            return l.chunkLocation == c.getChunkCoordinates();
                        });
                        chunkLocIt->update = true;
                    }
                    return;
                }
            }        
        }
    }
    
    Chunk c = Chunk(floor((float)pos.x / (float)size[0]), floor((float)pos.z / (float)size[2]));
    c.setBlockAtLocation(pos, block);
    rawBlockData.push_back(c);
}

bool BlockArrayData::isChunkLoaded(Chunk c) {
    return std::find_if(loadedChunkLocations.begin(), loadedChunkLocations.end(), [c](LoadedChunkInfo l) {
        return l.chunkLocation == c.getChunkCoordinates();
    }) != loadedChunkLocations.end();
}

void BlockArrayData::removeBlockAtPosition(BlockPos pos) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
    
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    rawBlockData.at(i).removeBlockAtLocation(pos);
                    if(isChunkLoaded(c)) {
                        std::vector<LoadedChunkInfo>::iterator chunkLocIt = std::find_if(loadedChunkLocations.begin(), loadedChunkLocations.end(), [c](LoadedChunkInfo l) {
                            return l.chunkLocation == c.getChunkCoordinates();
                        });
                        chunkLocIt->update = true;
                    }
                }
            }
        }
    }
}

//do not modify
BlockData BlockArrayData::getBlockAtPosition(BlockPos pos) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
        
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    return rawBlockData.at(i).getBlockAtLocation(pos);
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
    return rawBlockData;
}

//do not modify
Chunk* BlockArrayData::getChunkWithBlock(BlockPos pos) {
    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < rawBlockData.size(); ++i) {
        BlockPos location = rawBlockData[i].getChunkCoordinates();
        BlockPos locationEnd = rawBlockData[i].getChunkCoordinates() + BlockPos(size[0], size[1], size[2]);
        
        if(location.x <= pos.x && locationEnd.x > pos.x) {
            if(location.y <= pos.y && locationEnd.y > pos.y) {
                if(location.z <= pos.z && locationEnd.z > pos.z) {
                    return &rawBlockData[i];
                }
            }
        }
    }

    return fakeChunk;
}

bool BlockArrayData::shouldUpdateRenderer() {
    for(int i = 0; i < loadedChunkLocations.size(); ++i) {
        if(loadedChunkLocations.at(i).update) {
            return true;
        }
    }
    return false;
}

void BlockArrayData::hasUpdatedRenderer() {
    for(int i = 0; i < loadedChunkLocations.size(); ++i) {
        loadedChunkLocations.at(i).update = false;
    }
}

const std::vector<LoadedChunkInfo> BlockArrayData::getLoadedChunkLocations() {
    return loadedChunkLocations;
}

//used when world is loaded to force a rendering update
void BlockArrayData::setAllLoadedChunksToBeUpdated() {
    for(int i = 0; i < loadedChunkLocations.size(); ++i) {
        loadedChunkLocations.at(i).update = true;
    }
}

bool BlockArrayData::isValidPosition(AABB playerAABB, float* ypos) {
    for(int i = 0; i < getLoadedChunkLocations().size(); ++i) {
        Chunk* c = getChunkWithBlock(getLoadedChunkLocations().at(i).chunkLocation);
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