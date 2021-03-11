#ifndef BLOCKRENDEREDMODEL_H
#define BLOCKRENDEREDMODEL_H

#include "BlockFace.h"
#include <array>

class RenderedModel;

class BlockRenderedModel {
    public:
        BlockRenderedModel(std::array<BlockFace, 6> _renderedBlockModel);

        RenderedModel toRenderedModel();

        void rotateX90();

        void rotateY90();
        
        void rotateZ90();

        std::array<BlockFace, 6> renderedBlockModel;
};
#endif