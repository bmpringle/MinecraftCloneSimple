#include "BlockData.h"

BlockData::BlockData(std::shared_ptr<Block> type, BlockPos pos) : pos(pos), type(type) {

}

BlockData::BlockData() : pos(BlockPos(0, 0, 0)), type(nullptr) {

}

BlockPos BlockData::getPos() {
    return pos;
}

void BlockData::setPos(BlockPos pos_) {
    pos = pos_;
}

AABB BlockData::getAABB() {
    AABB aabb = type->getAABB();
    aabb.add(pos);
    return aabb;
}

std::shared_ptr<Block> BlockData::getBlockType() {
    return type;
}