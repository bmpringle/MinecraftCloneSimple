#ifndef RENDERCHUNKBUFFER_H
#define RENDERCHUNKBUFFER_H

#include <vector>
#include "Block.h"

class RenderChunkBuffer {
    public:
        RenderChunkBuffer(std::vector<float> _renderData, BlockPos _pos);

        RenderChunkBuffer();

        void setRenderData(std::vector<float> newData);

        unsigned int getRenderDataBuffer();

        int getRenderDataSize();

        BlockPos getPos();

        void renderChunk();

    private:
        std::vector<float> renderData = std::vector<float>();

        BlockPos pos = BlockPos(0, 0, 0);

        int chunkBufferSize = 0;

        //hey, this might leak memory because destructor doesn't call delete or something, ngl i have no clue
        unsigned int VBO = -1;

        unsigned int VAO = -1;
};
#endif