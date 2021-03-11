#ifndef BLOCKFACE_H
#define BLOCKFACE_H
#include "RenderedTriangle.h"
#include <array>
#include "SideEnum.h"

class BlockFace {
    public:
        BlockFace(std::array<RenderedTriangle, 2> tris, SideEnum s);

        void rotateX90(double x, double y, double z);

        void rotateY90(double x, double y, double z);

        void rotateZ90(double x, double y, double z);

        std::array<RenderedTriangle, 2> triangles;
        SideEnum side;
};
#endif