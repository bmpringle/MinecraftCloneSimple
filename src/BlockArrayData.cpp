#include "BlockArrayData.h"
#include <iostream>

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize) : rawBlockData(std::vector<Chunk>()) {
    size[0] = xSize; 
    size[1] = ySize; 
    size[2] = zSize;
}

void BlockArrayData::updateBlockAtPosition(BlockPos pos) {

}

void BlockArrayData::setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block) {
    //std::cout << rawBlockData.size() << std::endl;

    std::array<int, 3> size = Chunk::getChunkSize();

    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        BlockPos chunkLocation = c.getChunkCoordinates();
      
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    rawBlockData.at(i).setBlockAtRelativeLocation(BlockPos(pos.x % size[0], pos.y % size[1], pos.z % size[2]), block);
                    return;
                }
            }        
        }
    }

    Chunk c = Chunk(pos.x / 10, pos.z / 10);
    c.setBlockAtRelativeLocation(BlockPos(pos.x % size[0], pos.y % size[1], pos.z % size[2]), block);
    rawBlockData.push_back(c);
}

void BlockArrayData::removeBlockAtPosition(BlockPos pos) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
    
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    rawBlockData.at(i).removeBlockAtRelativeLocation(BlockPos(pos.x % size[0], pos.y % size[1], pos.z % size[2]));
                }
            }
        }
    }
}

std::shared_ptr<Block> BlockArrayData::getBlockAtPosition(BlockPos pos) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        Chunk c = rawBlockData.at(i);
        std::array<int, 3> size = Chunk::getChunkSize();
        BlockPos chunkLocation = c.getChunkCoordinates();
    
        if(pos.x >= chunkLocation.x && pos.x < chunkLocation.x + size[0]) {
            if(pos.y >= chunkLocation.y && pos.y < chunkLocation.y + size[1]) {
                if(pos.z >= chunkLocation.z && pos.z < chunkLocation.z + size[2]) {
                    return rawBlockData.at(i).getBlockAtRelativeLocation(BlockPos(pos.x % size[0], pos.y % size[1], pos.z % size[2]));
                }
            }
        }
    }
    return nullptr;
}

std::vector<Chunk> BlockArrayData::getRawChunkArray() {
    return rawBlockData;
}

Chunk BlockArrayData::getChunkWithBlock(BlockPos pos) {
    std::array<int, 3> size = Chunk::getChunkSize();

    for(Chunk c : rawBlockData) {
        BlockPos location = c.getChunkCoordinates();
        BlockPos locationEnd = c.getChunkCoordinates() + BlockPos(size[0], size[1], size[2]);
        
        if(location.x <= pos.x && locationEnd.x > pos.x) {
            if(location.y <= pos.y && locationEnd.y > pos.y) {
                if(location.z <= pos.z && locationEnd.z > pos.z) {
                    return c;
                }
            }
        }
    }
    //this is maybe not the best idea but i'm lazy right now
    return Chunk(0, 0);
}