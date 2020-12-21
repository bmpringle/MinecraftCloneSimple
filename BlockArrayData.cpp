#include "BlockArrayData.h"

BlockArrayData::BlockArrayData(int xSize, int ySize, int zSize) : rawBlockData(std::vector<Block>()) {
    size[0] = xSize; 
    size[1] = ySize; 
    size[2] = zSize;
}

void BlockArrayData::updateBlockAtPosition(BlockPos pos) {

}