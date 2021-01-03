#include "Chunk.h"
#include <iostream>

Chunk::Chunk(int xLoc, int zLoc) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(xLoc, 0, zLoc, 10, 100, 10) {
    for(int x = 0; x < 10; ++x) {
        for(int y = 0; y < 100; ++y) {
            for(int z = 0; z < 10; ++z) {
                if(blocksInChunk[x][y][z] == nullptr) {
                    std::cout << x << ", " << y << ", " << z << " is a nullptr";
                }
            }
        }
    }
}

std::shared_ptr<Block> Chunk::getBlockAtRelativeLocation(BlockPos pos) {
    if(pos.x >= 0 && pos.x < 10) {
        if(pos.y >= 0 && pos.y < 100) {
            if(pos.z >= 0 && pos.z < 10) {
                std::shared_ptr<Block> block = blocksInChunk[pos.x][pos.y][pos.z];
                block->setPos(chunkCoordinates + pos);
                return block;
            }
        }
    }
    return nullptr;
}

void Chunk::setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(pos.x >= 0 && pos.x < 10) {
        if(pos.y >= 0 && pos.y < 100) {
            if(pos.z >= 0 && pos.z < 10) {
                block->setPos(chunkCoordinates + pos);
                blocksInChunk[pos.x][pos.y][pos.z] = block;
            }
        }
    }
}

BlockPos Chunk::getChunkCoordinates() {
    return chunkCoordinates;
}

AABB Chunk::getChunkAABB() {
    return chunkAABB;
}

std::array<std::array<std::array<std::shared_ptr<Block>, 10>, 100>, 10> Chunk::getBlocksInChunk() {
    return blocksInChunk;
}