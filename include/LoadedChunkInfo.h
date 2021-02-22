#ifndef LOADEDCHUNKINFO_H
#define LOADEDCHUNKINFO_H
#include "Block.h"
#include "RenderChunkBuffer.h"

class LoadedChunkInfo {
    public:
        LoadedChunkInfo();
        LoadedChunkInfo(BlockPos loc, bool _update);
        BlockPos chunkLocation = BlockPos(0, 0, 0);
        bool update = true;
        RenderChunkBuffer* chunkBuffer = nullptr;
};
#endif