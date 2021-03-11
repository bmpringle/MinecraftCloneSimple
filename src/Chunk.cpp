#include "Chunk.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include "World.h"

Chunk::Chunk(int xLoc, int zLoc) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(getChunkCoordinates().x, getChunkCoordinates().y, getChunkCoordinates().z, X, Y, Z) {
    initTree();
}

Chunk::Chunk(int xLoc, int zLoc, bool _isFakeChunk) : chunkCoordinates(BlockPos(xLoc, 0, zLoc)), chunkAABB(getChunkCoordinates().x, getChunkCoordinates().y, getChunkCoordinates().z, X, Y, Z), blockTree(BinaryTree<SBDA, AABB, SBDA>()) {
    isFake = _isFakeChunk;
    initTree();
}

BlockData Chunk::getBlockAtLocation(BlockPos pos) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                AABB bAABB = AABB(pos.x, pos.y, pos.z, 1, 1, 1);
                std::function<bool(AABB, bool, std::optional<SBDA>&)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA>& block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                return blocks->value()[pos.y];
            }
        }
    }
    return BlockData();
}

BlockData& Chunk::getBlockReferenceAtLocation(BlockPos pos) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                AABB bAABB = AABB(pos.x, pos.y, pos.z, 1, 1, 1);
                std::function<bool(AABB, bool, std::optional<SBDA>&)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA>& block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                return blocks->value()[pos.y];
            }
        }
    }
    return dummyData;
}

void Chunk::setBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                BlockData blockData = BlockData(block, pos);
                AABB bAABB = blockData.getAABB();

                std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                blocks->value()[pos.y] = blockData;
            }
        }
    }
}

void Chunk::setBlockDataAtLocation(BlockPos pos, BlockData data) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                AABB bAABB = data.getAABB();
                data.setPos(pos);
                std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                blocks->value()[pos.y] = data;
            }
        }
    }
}

void Chunk::setColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                BlockData blockData = BlockData(block[0], pos);
                AABB bAABB = blockData.getAABB();

                std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                if(block.size() != amount.size()) {
                    std::cout << "assert failed: block.size() != amount.size()" << std::endl;
                    abort();
                }

                int b = 0;
                for(int _amount : amount) {
                    std::shared_ptr<Block> _block = block.at(b);
                    for(int y = pos.y; y < pos.y + _amount; ++y) {
                        BlockData data = BlockData(_block, BlockPos(pos.x, y, pos.z));
                        blocks->value()[y] = data;
                    }
                    pos.y += _amount;    
                    ++b;
                }
            }
        }
    }
}

void Chunk::softSetBlockAtLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                BlockData blockData = BlockData(block, pos);
                AABB bAABB = blockData.getAABB();

                std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                if(blocks->value()[pos.y].getBlockType() == nullptr) {
                    blocks->value()[pos.y] = blockData;
                }   
            }
        }
    }
}

void Chunk::softSetColumnOfBlocks(BlockPos pos, std::vector<std::shared_ptr<Block>> block, std::vector<int> amount) {
    if(pos.x >= getChunkCoordinates().x && pos.x < getChunkCoordinates().x + X) {
        if(pos.y >= getChunkCoordinates().y && pos.y < getChunkCoordinates().y + Y) {
            if(pos.z >= getChunkCoordinates().z && pos.z < getChunkCoordinates().z + Z) {
                BlockData blockData = BlockData(block[0], pos);
                AABB bAABB = blockData.getAABB();

                std::function<bool(AABB, bool, std::optional<SBDA>)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA> block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                if(block.size() != amount.size()) {
                    std::cout << "assert failed: block.size() != amount.size()" << std::endl;
                    abort();
                }

                int b = 0;
                for(int _amount : amount) {
                    std::shared_ptr<Block> _block = block.at(b);
                    for(int y = pos.y; y < pos.y + _amount; ++y) {
                        if(blocks->value()[y].getBlockType() == nullptr) {
                            BlockData data = BlockData(_block, BlockPos(pos.x, y, pos.z));
                            blocks->value()[y] = data;
                        }
                    }
                    pos.y += _amount;    
                    ++b;
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

std::vector<BlockData> Chunk::getBlocksInChunk() {
    std::function<bool(AABB, bool, std::optional<SBDA>&)> eval = [](AABB aabb, bool isLeaf, std::optional<SBDA>& block) -> bool { 
        return true;
    };
    std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);

    std::vector<BlockData> result = std::vector<BlockData>();

    for(std::optional<SBDA>* blocks : blocksVector) {
        for(int i = 0; i < 256; ++i) {
            if(blocks->value()[i].getBlockType() != nullptr) {
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
                std::function<bool(AABB, bool, std::optional<SBDA>&)> eval = [bAABB](AABB aabb, bool isLeaf, std::optional<SBDA>& block) -> bool { 
                    if(AABBIntersectedByAABB(bAABB, aabb)){
                        return true;
                    }
                    return false;
                };
                std::vector<std::optional<SBDA>*> blocksVector = blockTree.getLeafOfTree(eval);
                if(blocksVector.size() != 1) {
                    std::cout << "abort! there are " << blocksVector.size() << " valid blocks" << std::endl;
                    abort();
                }
                std::optional<SBDA>* blocks = blocksVector.at(0);
                blocks->value()[pos.y] = BlockData();
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
    SBDA nullarray = SBDA();
    nullarray.array.fill(BlockData());
    blockTree = BinaryTree<SBDA, AABB, SBDA>(leftCreate, rightCreate, getChunkAABB(), getChunkAABB(), nullarray);
}

BinaryTree<SBDA, AABB, SBDA>* Chunk::getBlockTree() {
    return &blockTree;
}