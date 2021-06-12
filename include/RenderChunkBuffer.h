#ifndef RENDERCHUNKBUFFER_H
#define RENDERCHUNKBUFFER_H

#include <vector>
#include "Block.h"

class RenderChunkBuffer {
    public:
        RenderChunkBuffer(std::vector<float> _renderData, BlockPos _pos);

        ~RenderChunkBuffer();

        void setRenderData(std::vector<float> newData);

        unsigned int getRenderDataBuffer();

        int getRenderDataSize();

        BlockPos getPos();

        void renderChunk();

    private:
        BlockPos pos = BlockPos(0, 0, 0);

        int chunkBufferSize = 0;

        unsigned int VBO = -1;

        unsigned int VAO = -1;
};
#endif