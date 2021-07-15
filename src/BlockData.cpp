#include "BlockData.h"
#include "BlockArrayData.h"
#include "World.h"
#include "ItemStack.h"

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
    AABB aabb = type->getAABB(data);
    aabb.add(pos);
    return aabb;
}

std::shared_ptr<Block> BlockData::getBlockType() {
    return type;
}

int BlockData::getData() {
    return data;
}

void BlockData::setData(int data) {
    this->data = data;
}

BlockRenderedModel BlockData::getRenderedModel() {
    if(type != nullptr) {
        BlockRenderedModel model = type->getRenderedModel(data);
        type->rotateModel(model, data);
        return model;
    }else {
        throw std::invalid_argument("type is null");
    }
}

std::string BlockData::getTextureName(SideEnum side) {
    if(type != nullptr) {
        return type->getTextureName(side, data);
    }else {
        throw std::invalid_argument("type is null");
    }
}

bool BlockData::isSolid() {
    if(type != nullptr) {
        return type->isSolid(data);
    }else {
        return false;
    }
}

bool BlockData::isOpaque() {
    if(type != nullptr) {
        return type->isOpaque(data);
    }else {
        return false;
    }
}

void BlockData::placedOnSide(SideEnum hPlacementAngle, SideEnum sideLookingAt) {
    if(type != nullptr) {
        type->onPlaced(hPlacementAngle, sideLookingAt, &data);
    }
}

void BlockData::updateBlock(BlockArrayData* data) {
    if(type != nullptr) {
        type->updateBlock(data, this);
    }
}

bool BlockData::activateBlock(World* world, ItemStack* stack) {
    if(type != nullptr) {
        return type->onBlockActivated(world, pos, stack, &data);
    }else {
        return false;
    }
}
