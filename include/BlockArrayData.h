#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"
#include <memory>
#include "RenderInclude.h"

class BlockArrayData {
    public:
        BlockArrayData(int xSize, int ySize, int zSize);

        void updateBlockAtPosition(BlockPos pos);

        void setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        std::vector<std::shared_ptr<Block>> getRawBlockArray();

        std::shared_ptr<Block> getBlockAtPosition(BlockPos pos);

        void removeBlockAtPosition(BlockPos pos);

    private:
        std::vector<std::shared_ptr<Block>> rawBlockData;
        int size[3];
};
#endif