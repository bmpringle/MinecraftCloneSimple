#ifndef BLOCKARRAYDATA_H
#define BLOCKARRAYDATA_H

#include <vector>
#include "Block.h"
#include <memory>
#include "RenderInclude.h"
#include "Chunk.h"

class BlockArrayData {
    public:
        BlockArrayData(int xSize, int ySize, int zSize);

        void updateBlockAtPosition(BlockPos pos);

        void setBlockAtPosition(BlockPos pos, std::shared_ptr<Block> block);

        std::vector<Chunk> getRawChunkArray();

        std::shared_ptr<Block> getBlockAtPosition(BlockPos pos);

        void removeBlockAtPosition(BlockPos pos);

        Chunk getChunkWithBlock(BlockPos pos);

    private:
        std::vector<Chunk> rawBlockData;
        int size[3];
};
#endif