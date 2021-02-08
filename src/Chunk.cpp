#include "Chunk.h"
#include <iostream>
#include <algorithm>

Chunk::Chunk(int xLoc, int zLoc) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(0, 0, 0, X, Y, Z) {

}

Chunk::Chunk(int xLoc, int zLoc, bool _isFakeChunk) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(0, 0, 0, X, Y, Z) {
    isFake = _isFakeChunk;
}

bool Chunk::doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block) {
    BlockPos bPos = block->getPos();
    if(bPos.x == pos.x && bPos.y == pos.y && bPos.z == pos.z) {
        return true;
    }
    return false;
}

std::shared_ptr<Block> Chunk::getBlockAtRelativeLocation(BlockPos pos) {
    if(pos.x >= 0 && pos.x < X) {
        if(pos.y >= 0 && pos.y < Y) {
            if(pos.z >= 0 && pos.z < Z) {
                std::vector<std::shared_ptr<Block>>::iterator iter = std::find_if(blocksInChunk.begin(), blocksInChunk.end(), [this, pos] (const std::shared_ptr<Block> b) { 
                    return this->doesBlockHaveCoordinates(getChunkCoordinates() + pos, b); 
                });

                if(iter != blocksInChunk.end()) {
                    std::shared_ptr<Block> block = *iter;
                    block->setPos(getChunkCoordinates() + pos);
                    return block;
                }
            }
        }
    }
    return nullptr;
}

void Chunk::setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(pos.x >= 0 && pos.x < X) {
        if(pos.y >= 0 && pos.y < Y) {
            if(pos.z >= 0 && pos.z < Z) {
                std::vector<std::shared_ptr<Block>>::iterator iter = std::find_if(blocksInChunk.begin(), blocksInChunk.end(), [this, pos] (const std::shared_ptr<Block> b) { 
                    return this->doesBlockHaveCoordinates(getChunkCoordinates() + pos, b); 
                });

                if(iter != blocksInChunk.end()) {
                    block->setPos(getChunkCoordinates() + pos);
                    *iter =  block;
                }else {
                    block->setPos(getChunkCoordinates() + pos);
                    blocksInChunk.push_back(block);
                }
            }
        }
    }
}

BlockPos Chunk::getChunkCoordinates() const {
    return BlockPos(chunkCoordinates.x * X, chunkCoordinates.y * Y, chunkCoordinates.z * Z);
}

AABB Chunk::getChunkAABB() {
    return chunkAABB;
}

std::vector<std::shared_ptr<Block>> Chunk::getBlocksInChunk() {
    return blocksInChunk;
}

std::array<int, 3> Chunk::getChunkSize() {
    std::array<int, 3> result = {X, Y, Z};
    return result;
}

void Chunk::removeBlockAtRelativeLocation(BlockPos pos) {
    if(pos.x >= 0 && pos.x < X) {
        if(pos.y >= 0 && pos.y < Y) {
            if(pos.z >= 0 && pos.z < Z) {
                std::vector<std::shared_ptr<Block>>::iterator iter = std::find_if(blocksInChunk.begin(), blocksInChunk.end(), [this, pos] (const std::shared_ptr<Block> b) { 
                    return this->doesBlockHaveCoordinates(getChunkCoordinates() + pos, b); 
                });

                if(iter != blocksInChunk.end()) {
                    blocksInChunk.erase(blocksInChunk.begin() + (iter - blocksInChunk.begin()));
                }
            }
        }
    }
}

bool Chunk::isFakeChunk() {
    return isFake;
}