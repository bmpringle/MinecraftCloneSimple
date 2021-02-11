#include "BlockTree.h"

BlockTree::BlockTree(int chunkSize) : node(BlockNode(BlockPos(0, 0, 0), 16, 16, true)){

}

std::shared_ptr<Block> BlockTree::getBlockAtRelativeLocation(BlockPos pos) {
    return node.getBlockAtRelativeLocation(pos);
}

void BlockTree::setBlockAtRelativeLocation(BlockPos pos, std::shared_ptr<Block> block) {
    node.setBlockAtRelativeLocation(pos, block);
}

void BlockTree::removeBlockAtRelativeLocation(BlockPos pos) {
    node.removeBlockAtRelativeLocation(pos);
}

std::vector<std::shared_ptr<Block>> BlockTree::getBlocksInChunk() {
    std::vector<std::shared_ptr<Block>> blocks = std::vector<std::shared_ptr<Block>>();
    node.getBlocksInChunk(&blocks);
    return blocks;
}

bool BlockTree::doesBlockHaveCoordinates(BlockPos pos, std::shared_ptr<Block> block) {
    return node.doesBlockHaveCoordinates(pos, block);
}
