#ifndef LOADEDCHUNKINFO_H
#define LOADEDCHUNKINFO_H
#include "Block.h"

class LoadedChunkInfo {
    public:
        LoadedChunkInfo();
        LoadedChunkInfo(BlockPos loc, int _update);
        BlockPos chunkLocation = BlockPos(0, 0, 0);
        int update = 1;
};
#endif