#include "Chunk.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include "World.h"

Chunk::Chunk(int xLoc, int zLoc) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(getChunkCoordinates().x, getChunkCoordinates().y, getChunkCoordinates().z, X, Y, Z) {
    initTree();
}

Chunk::Chunk(int xLoc, int zLoc, bool _isFakeChunk) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(getChunkCoordinates().x, getChunkCoordinates().y, getChunkCoordinates().z, X, Y, Z), blockTree(BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>>()) {
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

std::shared_ptr<Block> Chunk::getBlockAtLocation(BlockPos pos) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                AABB bAABB = AABB(pos.x, pos.y, pos.z, 1, 1, 1);
                std::function<bool(AABB, bool, std::optional<std::array<std::shared_ptr<Block>, 256>>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<std::array<std::shared_ptr<Block>, 256>> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<std::array<std::shared_ptr<Block>, 256>>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<std::array<std::shared_ptr<Block>, 256>>* blocks = blocksVector.at(0);
                return blocks->value()[pos.y];
            }
        }
    }
    return nullptr;
}

void Chunk::setBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                block->setPos(pos);
                AABB bAABB = block->getAABB();
                std::function<bool(AABB, bool, std::optional<std::array<std::shared_ptr<Block>, 256>>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<std::array<std::shared_ptr<Block>, 256>> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<std::array<std::shared_ptr<Block>, 256>>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<std::array<std::shared_ptr<Block>, 256>>* blocks = blocksVector.at(0);
                blocks->value()[pos.y] = block;
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
    std::function<bool(AABB, bool, std::optional<std::array<std::shared_ptr<Block>, 256>>)> eval = [](AABB aabb, bool isLeaf, std::optional<std::array<std::shared_ptr<Block>, 256>> block) -> bool { 
        return true;
    };
    std::vector<std::optional<std::array<std::shared_ptr<Block>, 256>>*> blocksVector = blockTree.getLeafOfTree(eval);

    std::vector<std::shared_ptr<Block>> result = std::vector<std::shared_ptr<Block>>();

    for(std::optional<std::array<std::shared_ptr<Block>, 256>>* blocks : blocksVector) {
        for(int i = 0; i < 256; ++i) {
            if(blocks->value()[i] != nullptr) {
                result.push_back(blocks->value()[i]);
            }
        }
    }
    return result;
}

std::array<int, 3> Chunk::getChunkSize() {
    std::array<int, 3> result = {X, Y, Z};
    return result;
}

void Chunk::removeBlockAtLocation(BlockPos pos) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                //blocktree version
                AABB bAABB = AABB(pos.x, pos.y, pos.z, 1, 1, 1);
                std::function<bool(AABB, bool, std::optional<std::array<std::shared_ptr<Block>, 256>>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<std::array<std::shared_ptr<Block>, 256>> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<std::array<std::shared_ptr<Block>, 256>>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<std::array<std::shared_ptr<Block>, 256>>* blocks = blocksVector.at(0);
                blocks->value()[pos.y] = nullptr;
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

BinaryTree<std::array<std::shared_ptr<Block>, 256>, AABB, std::array<std::shared_ptr<Block>, 256>>* Chunk::getBlockTree() {
    return &blockTree;
}