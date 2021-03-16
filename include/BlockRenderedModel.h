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

        void rotateX90(double x, double y, double z);

        void rotateY90(double x, double y, double z);
        
        void rotateZ90(double x, double y, double z);

        std::array<BlockFace, 6> renderedBlockModel;
};
#endif