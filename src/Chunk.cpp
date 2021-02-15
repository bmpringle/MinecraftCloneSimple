#include "Chunk.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include "World.h"

Chunk::Chunk(int xLoc, int zLoc) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(0, 0, 0, X, Y, Z) {
    initTree();
}

Chunk::Chunk(int xLoc, int zLoc, bool _isFakeChunk) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(0, 0, 0, X, Y, Z), blockTree(BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>>()) {
    isFake = _isFakeChunk;
    initTree();
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
                block->setPos(getChunkCoordinates() + pos);
                AABB bAABB = block->getAABB();
                bAABB.add(pos);
                std::function<bool(AABB)> eval = [bAABB](AABB aabb) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                /*std::vector<std::optional<std::array<std::shared_ptr<Block>, 256>>*> block = blockTree.getLeafOfTree(eval);
                if(block.size() != 1) {
                    std::cout << "abort! there are " << block.size() << " valid blocks" << std::endl;
                    abort();
                }*/
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

void Chunk::initTree() {
    std::function<bool(AABB*)> leftCreate = [](AABB* aabb) -> bool { 
        if(aabb->xSize > 1 && aabb->zSize > 1) {
            if(aabb->xSize > aabb->zSize) {
                aabb->xSize = aabb->xSize / 2;
                return true;
            }else {
                aabb->zSize = aabb->zSize / 2;
                return true;
            }
        }else if(aabb->xSize > 1) {
            aabb->xSize = aabb->xSize / 2;
            return true;
        }else if(aabb->zSize > 1) {
            aabb->zSize = aabb->zSize / 2;
            return true;
        }else {
            return false;
        }
    };

    std::function<bool(AABB*)> rightCreate = [](AABB* aabb) -> bool { 
        if(aabb->xSize > 1 && aabb->zSize > 1) {
            if(aabb->xSize > aabb->zSize) {
                aabb->xSize = aabb->xSize / 2;
                aabb->startX = aabb->startX + aabb->xSize;
                return true;
            }else {
                aabb->zSize = aabb->zSize / 2;
                aabb->startZ = aabb->startZ + aabb->zSize;
                return true;
            }
        }else if(aabb->xSize > 1) {
            aabb->xSize = aabb->xSize / 2;
            aabb->startX = aabb->startX + aabb->xSize;
            return true;
        }else if(aabb->zSize > 1) {
            aabb->zSize = aabb->zSize / 2;
            aabb->startZ = aabb->startZ + aabb->zSize;
            return true;
        }else {
            return false;
        }
    };
    std::array<std::shared_ptr<Block>, 256> nullarray = {nullptr};
    nullarray.fill(nullptr);
    blockTree = BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>>(leftCreate, rightCreate, getChunkAABB(), getChunkAABB(), nullarray);
}