#ifndef LOADEDCHUNKINFO_H
#define LOADEDCHUNKINFO_H
#include "Block.h"
#include "RenderChunkBuffer.h"

class LoadedChunkInfo {
    public:
        LoadedChunkInfo(BlockPos loc, bool _update);
        BlockPos chunkLocation;
        bool update = true;
        RenderChunkBuffer* chunkBuffer = nullptr;
};
#endif