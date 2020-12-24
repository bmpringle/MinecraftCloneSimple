#include "Block.h"

std::string Block::getName() {
    return "block";
}

BlockPos Block::getPos() {
    return pos;
}

void Block::setPos(BlockPos pos_) {
    pos = pos_;
}

Block::Block() : pos(BlockPos(0, 0, 0)) {

}

AABB Block::getAABB() {
    return AABB(0, 0, 0, 1, 1, 1);
}