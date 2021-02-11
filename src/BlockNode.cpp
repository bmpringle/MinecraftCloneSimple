#include "BlockNode.h"

BlockNode::BlockNode(BlockPos pos, int xSize, int zSize, bool recursiveFlag) : chunkLocation(pos) {
    this->xSize = xSize;
    this->zSize = zSize;

    if(xSize + xSize > 1) {
        BlockPos differentLocation = BlockPos(-1, -1, -1);
        int differentXSize;
        int differentZSize;

        if(recursiveFlag) {
            differentLocation = BlockPos(chunkLocation.x + xSize / 2, chunkLocation.y, chunkLocation.z);
            differentXSize = xSize / 2;
            differentZSize = zSize;
        }else {
            differentLocation = BlockPos(chunkLocation.x, chunkLocation.y, chunkLocation.z + zSize / 2);
            differentXSize = xSize;
            differentZSize = zSize / 2;
        }
        *left = BlockNode(chunkLocation, differentXSize, differentZSize, !recursiveFlag);
        *right = BlockNode(differentLocation, differentXSize, differentZSize, !recursiveFlag);
    }
}

std::shared_ptr<Block> BlockNode::getBlockAtRelativeLocation(BlockPos pos) {
    if(left != nullptr) {
        if(pos.x >= left->getNodeLocation().x && pos.x < left->getNodeLocation().x + xSize / 2) {
            if(pos.y >= left->getNodeLocation().y && pos.y < left->getNodeLocation().y + 256) {
                if(pos.z >= left->getNodeLocation().z && pos.z < left->getNodeLocation().z + zSize / 2) {
                    return left->getBlockAtRelativeLocation(pos);
                }
            }
        }
        return right->getBlockAtRelativeLocation(pos);
    }else {
        return block;
    }
}

void BlockNode::setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block) {
    if(left != nullptr) {
        if(pos.x >= left->getNodeLocation().x && pos.x < left->getNodeLocation().x + left->getAABB().xSize) {
            if(pos.y >= left->getNodeLocation().y && pos.y < left->getNodeLocation().y + 256) {
                if(pos.z >= left->getNodeLocation().z && pos.z < left->getNodeLocation().z + left->getAABB().zSize) {
                    left->setBlockAtRelativeLocation(pos, block);
                    return;
                }
            }
        }
        if(pos.x >= right->getNodeLocation().x && pos.x < right->getNodeLocation().x + right->getAABB().xSize) {
            if(pos.y >= right->getNodeLocation().y && pos.y < right->getNodeLocation().y + 256) {
                if(pos.z >= right->getNodeLocation().z && pos.z < right->getNodeLocation().z + right->getAABB().zSize) {
                    right->setBlockAtRelativeLocation(pos, block);
                }
            }
        }
    }else {
        this->block = block;
        this->block->setPos(pos);
    }
}

void BlockNode::removeBlockAtRelativeLocation(BlockPos pos) {
    if(left != nullptr) {
        if(pos.x >= left->getNodeLocation().x && pos.x < left->getNodeLocation().x + left->getAABB().xSize) {
            if(pos.y >= left->getNodeLocation().y && pos.y < left->getNodeLocation().y + 256) {
                if(pos.z >= left->getNodeLocation().z && pos.z < left->getNodeLocation().z + left->getAABB().zSize) {
                    left->setBlockAtRelativeLocation(pos, block);
                }
            }
        }
        right->setBlockAtRelativeLocation(pos, block);
    }else {
        block = nullptr;
    }
}

void BlockNode::getBlocksInChunk(std::vector<std::shared_ptr<Block>>* blocks) {
    if(xSize + zSize > 1) {
        left->getBlocksInChunk(blocks);
        right->getBlocksInChunk(blocks);
    }else {
        blocks->push_back(block);
        chunkLocation.print();
    }
}

bool BlockNode::doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block) {
    BlockPos bPos = block->getPos();
    if(bPos.x == pos.x && bPos.y == pos.y && bPos.z == pos.z) {
        return true;
    }
    return false;
}

AABB BlockNode::getAABB() {
    return AABB(chunkLocation.x, chunkLocation.y, chunkLocation.z, xSize, 256, zSize);
}

BlockPos BlockNode::getNodeLocation() {
    return chunkLocation;
}