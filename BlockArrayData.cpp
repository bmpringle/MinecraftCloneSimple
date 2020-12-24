#include "BlockArrayData.h"

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize) : rawBlockData(std::vector<std::shared_ptr<Block>>()) {
    size[0] = xSize; 
    size[1] = ySize; 
    size[2] = zSize;
}

void BlockArrayData::updateBlockAtPosition(BlockPos pos) {

}

void BlockArrayData::setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        BlockPos p = rawBlockData.at(i)->getPos();
        if(p.x == pos.x && p.y == pos.y && p.z == pos.z) {
            rawBlockData.erase(rawBlockData.begin() + i - 1);
        }
    }
    block->setPos(pos);
    rawBlockData.push_back(block);
}

std::shared_ptr<Block> BlockArrayData::getBlockAtPosition(BlockPos pos) {
    for(int i = 0; i < rawBlockData.size(); ++i) {
        BlockPos p = rawBlockData.at(i)->getPos();
        if(p.x == pos.x && p.y == pos.y && p.z == pos.z) {
            return rawBlockData.at(i);
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Block>> BlockArrayData::getRawBlockArray() {
    return rawBlockData;
}