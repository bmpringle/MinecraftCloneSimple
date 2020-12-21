#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"

class BlockArrayData {
    public:
        BlockArrayData(int xSize, int ySize, int zSize);

        void updateBlockAtPosition(BlockPos pos);
    private:
        std::vector<Block> rawBlockData;
        int size[3];
};
#endif