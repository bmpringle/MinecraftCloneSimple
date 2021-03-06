#include "BlockArrayData.h"
#include <iostream>
#include <math.h>
#include "World.h"
#include <algorithm>
#include <Blocks.h>
#include "PlatformFilesystem.h"

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize, std::string _worldFolder, int seed) : chunkList(std::vector<Chunk>()), SEED(seed), worldFolder(_worldFolder) {
    size[0] = xSize; 
    size[1] = ySize; 
    size[2] = zSize;
    noise.SetSeed(SEED);
}

void BlockArrayData::sendChunkUpdates() {
    for(Chunk& chunk : chunkList) {
        if(loadedChunkLocations.count(chunk.getChunkCoordinates()) > 0) {
            chunk.updateChunk(this);
        }
    }
}

void BlockArrayData::updateLoadedChunks(BlockPos pos, World* world) {   

    Chunk* centerChunk = getChunkWithBlock(pos);

    int renderDistance = world->getChunkRenderDistance();
    std::array<int, 3> size = Chunk::getChunkSize();

    std::map<BlockPos, LoadedChunkInfo> oldChunks = loadedChunkLocations;

    loadedChunkLocations.clear();

    for(float x = -(float)renderDistance; x <= (float)renderDistance; ++x) {
        for(float z = -(float)renderDistance; z <= (float)renderDistance; ++z) {
            BlockPos playerBlock = BlockPos((int)centerChunk->getChunkCoordinates().x + size[0] * x, 0, (int)centerChunk->getChunkCoordinates().z + size[2] * z);
            if(!getChunkWithBlock(playerBlock)->isFakeChunk()) {
                BlockPos chunkLocation = getChunkWithBlock(playerBlock)->getChunkCoordinates();
                LoadedChunkInfo info = LoadedChunkInfo(BlockPos(0, 0, 0), false);
                if(oldChunks.count(chunkLocation) > 0) {
                    info = oldChunks.at(chunkLocation);
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, info.update);
                }else {
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, true);
                }
            }else {
                int x = floor((float)playerBlock.x / (float)Chunk::getChunkSize()[0]);
                int z = floor((float)playerBlock.z / (float)Chunk::getChunkSize()[2]);
                std::string chunkPath = worldFolder+"/data/"+std::to_string(x)+"-"+std::to_string(z)+".cdat";
                if(fs::exists(chunkPath)) {
                    loadChunkFromFile(chunkPath, playerBlock);
                }else {
                    generateChunk(playerBlock);
                }

                LoadedChunkInfo info = LoadedChunkInfo(BlockPos(0, 0, 0), false);
                BlockPos chunkLocation = getChunkWithBlock(playerBlock)->getChunkCoordinates();
                if(oldChunks.count(chunkLocation) > 0) {
                    info = oldChunks.at(chunkLocation);       
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, info.update);
                }else {
                    loadedChunkLocations[chunkLocation] = LoadedChunkInfo(chunkLocation, true);
                }
            }
        }
    }

    for(std::pair<BlockPos, LoadedChunkInfo> chunkPos : oldChunks) {
        if(loadedChunkLocations.count(chunkPos.first) == 0) {
            unloadChunkToFile(chunkPos.first, world->getName());
        }
    }
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
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
                    }
                    return;
                }
            }        
        }
    }
    generateChunk(BlockPos((float)size[0] * floor((float)pos.x / (float)size[0]), 0, (float)size[2] * floor((float)pos.z / (float)size[2])));
    setBlockAtPosition(pos, block);
}

void BlockArrayData::setBlockDataAtPosition(BlockPos pos, BlockData data) {
    std::array<int, 3> size = Chunk::getChunkSize();
    
    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).setBlockDataAtLocation(pos, data);
                    if(isChunkLoaded(c)) {
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
                    }
                    return;
                }
            }        
        }
    }
    generateChunk(BlockPos((float)size[0] * floor((float)pos.x / (float)size[0]), 0, (float)size[2] * floor((float)pos.z / (float)size[2])));
    setBlockDataAtPosition(pos, data);
}

void BlockArrayData::softSetBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block) {
    std::array<int, 3> size = Chunk::getChunkSize();

    
    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).softSetBlockAtLocation(pos, block);
                    if(isChunkLoaded(c)) {
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
                    }
                    return;
                }
            }        
        }
    }
    generateChunk(BlockPos((float)size[0] * floor((float)pos.x / (float)size[0]), 0, (float)size[2] * floor((float)pos.z / (float)size[2])));
    softSetBlockAtPosition(pos, block);
}

void BlockArrayData::setColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount) {
    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).setColumnOfBlocks(pos, block, amount);
                    if(isChunkLoaded(c)) {
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
                    }
                    return;
                }
            }        
        }
    }   
    generateChunk(BlockPos((float)size[0] * floor((float)pos.x / (float)size[0]), 0, (float)size[2] * floor((float)pos.z / (float)size[2])));
    setColumnAtPosition(pos, block, amount);
}

void BlockArrayData::softSetColumnAtPosition(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount) {
    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    chunkList.at(i).softSetColumnOfBlocks(pos, block, amount);
                    if(isChunkLoaded(c)) {
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
                    }
                    return;
                }
            }        
        }
    }
    generateChunk(BlockPos((float)size[0] * floor((float)pos.x / (float)size[0]), 0, (float)size[2] * floor((float)pos.z / (float)size[2])));
    softSetColumnAtPosition(pos, block, amount);
}

bool BlockArrayData::isChunkLoaded(Chunk c) {
    if(loadedChunkLocations.count(c.getChunkCoordinates()) > 0) {
        return true;
    }
    return false;
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
                        ++loadedChunkLocations[c.getChunkCoordinates()].update;
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

BlockData& BlockArrayData::getBlockReferenceAtPosition(BlockPos pos) {
    for(int i = 0; i < chunkList.size(); ++i) {
        Chunk c = chunkList.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
        
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    return chunkList.at(i).getBlockReferenceAtLocation(pos);
                }
            }
        }
    }
    return dummyData;
}

/*
 * do not modify. actually, just don't use. 
 * make another member function for whatever 
 * purpose you need, copying all this data is 
 * just not a fun time. ever.
 */
std::vector<Chunk>& BlockArrayData::getChunkArrayReference() {
    return chunkList;
}

//do not modify
Chunk* BlockArrayData::getChunkWithBlock(BlockPos pos) {
    
    std::array<int, 3> size = Chunk::getChunkSize();

    BlockPos chunkEnd = BlockPos(size[0], size[1], size[2]);
    
    for(int i = 0; i < chunkList.size(); ++i) {
        BlockPos location = chunkList[i].getChunkCoordinates();
        
        if(location.x <= pos.x && location.x + chunkEnd.x > pos.x) {
            if(location.y <= pos.y && location.y + chunkEnd.y > pos.y) {
                if(location.z <= pos.z && location.z + chunkEnd.z > pos.z) {
                    return &chunkList[i];
                }
            }
        }
    }

    return fakeChunk;
}

bool BlockArrayData::shouldUpdateRenderer() {
    for(std::pair<const BlockPos, LoadedChunkInfo>& pair : loadedChunkLocations) {
        if(pair.second.update > 0) {
            return true;
        }
    }
    return false;
}

void BlockArrayData::hasUpdatedRenderer() {
    for(std::pair<const BlockPos, LoadedChunkInfo>& pair : loadedChunkLocations) {
        if(pair.second.update > 0) {
            --pair.second.update;
        }
    }
}

const std::map<BlockPos, LoadedChunkInfo> BlockArrayData::getLoadedChunkLocations() {
    return loadedChunkLocations;
}

//used when world is loaded to force a rendering update
void BlockArrayData::setAllLoadedChunksToBeUpdated() {
    for(std::pair<const BlockPos, LoadedChunkInfo>& pair : loadedChunkLocations) {
        ++pair.second.update;
    }
}

bool BlockArrayData::isValidPosition(AABB playerAABB, float* ypos) {
    for(std::pair<const BlockPos, LoadedChunkInfo>& pair : loadedChunkLocations) {
        Chunk* c = getChunkWithBlock(pair.first);
        if(!c->isFakeChunk()) {
            AABB chunkAABB = c->getChunkAABB();
            if(AABBIntersectedByAABB(playerAABB, chunkAABB)) {
                auto tree = c->getBlockTree();
                std::function<bool(AABB, bool, std::optional<SBDA>*)> eval = [playerAABB](AABB aabb, bool isLeaf, std::optional<SBDA>* block) -> bool {
                    if(AABBIntersectedByAABB(playerAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = tree->getLeafOfTree(eval);
                if(blocksVector.size() > 0) {
                    for(std::optional<SBDA>* blocksColumn : blocksVector) {
                        for(int i = 0; i < 256; ++i) {
                            BlockData block = (*blocksColumn).value()[i];
                            if(block.getBlockType() != nullptr && block.isSolid()) {
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

void spawnTree(BlockArrayData* data, Chunk* chunk, int height, BlockPos beginningOfTree) {
    std::vector<std::shared_ptr<Block>> blocks = {Blocks::log};
    std::vector<int> amounts = {height};
    chunk->softSetColumnOfBlocks(beginningOfTree, blocks, amounts);

    //top
    chunk->softSetBlockAtLocation(BlockPos(beginningOfTree.x, beginningOfTree.y + amounts[0], beginningOfTree.z), Blocks::leaf);

    blocks = {Blocks::leaf};
    std::vector<int> amountstwo = {2};
    std::vector<int> amountsone = {1};
    //ring 1
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(1, height - 2, 0), blocks, amountstwo);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(1, height - 2, 1), blocks, amountsone);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(0, height - 2, 1), blocks, amountstwo);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(-1, height - 2, 1), blocks, amountsone);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(-1, height - 2, 0), blocks, amountstwo);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(-1, height - 2, -1), blocks, amountsone);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(0, height - 2, -1), blocks, amountstwo);
    data->softSetColumnAtPosition(beginningOfTree + BlockPos(1, height - 2, -1), blocks, amountsone);
}

void BlockArrayData::generateChunk(BlockPos chunkLocation) {
    Chunk generatingChunk = Chunk(floor((float)chunkLocation.x / (float)Chunk::getChunkSize()[0]), floor((float)chunkLocation.z / (float)Chunk::getChunkSize()[2]));

    std::map<BlockPos, int> treeMap = std::map<BlockPos, int>();

    for(int x = chunkLocation.x; x < chunkLocation.x + width; ++x) {
        for(int z = chunkLocation.z; z < chunkLocation.z + height; ++z) {
            double nx = x/width - 0.5, ny = z/height - 0.5;
            double value = 1 * noise.GetValue(1 * nx, 1 * ny, zNoise);
            int blockHeight = floor(value * amplifier) + averageTerrainHeight;
            std::vector<std::shared_ptr<Block>> blocks = {Blocks::cobblestone, Blocks::dirt, Blocks::grass};
            std::vector<int> amounts = {blockHeight - 4, 3, 1};
            generatingChunk.setColumnOfBlocks(BlockPos(x, 0, z), blocks, amounts);
            
            if(blockHeight < waterLevel) {
                std::vector<std::shared_ptr<Block>> blocks = {Blocks::dirt, Blocks::water};
                std::vector<int> amounts = {1, waterLevel - blockHeight};
                generatingChunk.setColumnOfBlocks(BlockPos(x, blockHeight - 1, z), blocks, amounts);
            }

            double tree = abs(rand() % 1000);

            if(tree < 5) {
                treeMap[BlockPos(x, blockHeight, z)] = ((abs(rand() % 2) == 1) ? 5 : 4); 
            }
        }
    }

    chunkList.push_back(generatingChunk);

    for(std::pair<const BlockPos, int>& pair: treeMap) {
        spawnTree(this, &generatingChunk, pair.second, pair.first);
    }
}

bool BlockArrayData::isAABBInWater(AABB playerAABB) {
    for(std::pair<const BlockPos, LoadedChunkInfo>& pair : loadedChunkLocations) {
        Chunk* c = getChunkWithBlock(pair.first);
        if(!c->isFakeChunk()) {
            AABB chunkAABB = c->getChunkAABB();
            if(AABBIntersectedByAABB(playerAABB, chunkAABB)) {
                auto tree = c->getBlockTree();
                std::function<bool(AABB, bool, std::optional<SBDA>*)> eval = [playerAABB](AABB aabb, bool isLeaf, std::optional<SBDA>* block) -> bool { 
                    if(AABBIntersectedByAABB(playerAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = tree->getLeafOfTree(eval);
                if(blocksVector.size() > 0) {
                    for(std::optional<SBDA>* blocksColumn : blocksVector) {
                        for(int i = 0; i < 256; ++i) {
                            BlockData block = (*blocksColumn).value()[i];
                            if(block.getBlockType() != nullptr && block.getBlockType()->getName() == "water") {
                                AABB blockAABB = block.getAABB();
                                if(AABBIntersectedByAABB(playerAABB, blockAABB)) {
                                    return true;
                                }
                            }
                        }
                    }
                }         
            }
        }
    }
    return false;
}

void BlockArrayData::setWorldFolder(std::string path) {
    worldFolder = path;
}

void BlockArrayData::loadChunkFromFile(std::string chunkPath, BlockPos chunkLocation) {
    Chunk generatingChunk = Chunk(floor((float)chunkLocation.x / (float)Chunk::getChunkSize()[0]), floor((float)chunkLocation.z / (float)Chunk::getChunkSize()[2]));
    std::ifstream t(chunkPath);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size); 

    auto tree = generatingChunk.getBlockTree();
    tree->deserialize(buffer);

    chunkList.push_back(generatingChunk);
}

int BlockArrayData::getSeed() {
    return SEED;
}

void BlockArrayData::setChunkToUpdate(BlockPos chunkLocation) {
    if(loadedChunkLocations.count(chunkLocation) > 0) {
        ++loadedChunkLocations.at(chunkLocation).update;
    }
}

void BlockArrayData::unloadChunkToFile(BlockPos chunkLocation, std::string worldname) {
    if(!fs::exists("./worlds/"+worldname+"/data/")) {
        fs::create_directories("./worlds/"+worldname+"/data/");
    }

    int i = 0;
    for(Chunk& c : chunkList) {
        if(c.getChunkCoordinates() == chunkLocation) {
            auto tree = c.getBlockTree();
            std::string data = tree->serialize();
            std::string dataname = std::to_string(c.getChunkCoordinates().x / Chunk::getChunkSize()[0]) + "-" + std::to_string(c.getChunkCoordinates().z / Chunk::getChunkSize()[2]);
            std::ofstream chunkFile("./worlds/"+worldname+"/data/"+dataname+".cdat");
            chunkFile.write(data.c_str(), data.length());

            chunkList.erase(chunkList.begin() + i);
            break;   
        }
        ++i;
    }
}
