#ifndef RENDERCHUNKBUFFER_H
#define RENDERCHUNKBUFFER_H

#include <vector>
#include "Block.h"
#include "ModelRegister.h"

class RenderChunkBuffer {
    public:
        RenderChunkBuffer(std::map<std::string, std::vector<int>> _renderData, BlockPos _pos, ModelRegister* modelRegister);

        ~RenderChunkBuffer();

        void setRenderData(std::map<std::string, std::vector<int>> newData, ModelRegister* modelRegister);

        BlockPos getPos();

        void renderChunk(ModelRegister* modelRegister);

    private:
        BlockPos pos = BlockPos(0, 0, 0);

        std::map<std::string, int> numberOfInstances = std::map<std::string, int>();
        std::map<std::string, unsigned int> renderData = std::map<std::string, unsigned int>();
};
#endif
