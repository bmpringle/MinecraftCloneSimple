#ifndef RENDERCHUNKBUFFER_H
#define RENDERCHUNKBUFFER_H

#include <vector>
#include "Block.h"

class RenderChunkBuffer {
    public:
        RenderChunkBuffer(std::vector<float> _renderDataO, std::vector<float> _renderDataT, BlockPos _pos);

        RenderChunkBuffer();

        void setRenderData(std::vector<float> newDataOpaque, std::vector<float> newDataTransparent);

        unsigned int getRenderDataBufferOpaque();

        unsigned int getRenderDataBufferTransparent();

        int getRenderDataSizeOpaque();

        int getRenderDataSizeTransparent();

        BlockPos getPos();

        void renderChunkOpaque();

        void renderChunkTransparent();

    private:
        std::vector<float> renderDataO = std::vector<float>();
        std::vector<float> renderDataT = std::vector<float>();

        BlockPos pos = BlockPos(0, 0, 0);

        int chunkBufferSizeO = 0;
        int chunkBufferSizeT = 0;

        //hey, this might leak memory because destructor doesn't call delete or something, ngl i have no clue
        unsigned int VBO_OPAQUE = -1;
        unsigned int VBO_TRANSPARENT = -1;

        unsigned int VAO_OPAQUE = -1;
        unsigned int VAO_TRANSPARENT = -1;
};
#endif