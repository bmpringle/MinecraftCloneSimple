#ifndef BLOCKFACE_H
#define BLOCKFACE_H

#include "RenderedTriangle.h"
#include <vector>
#include "SideEnum.h"

class BlockFace {
    public:
        BlockFace(std::vector<RenderedTriangle> tris, SideEnum s);

        void rotateX90(double x, double y, double z);

        void rotateY90(double x, double y, double z);

        void rotateZ90(double x, double y, double z);

        std::vector<RenderedTriangle> triangles;
        SideEnum side;
};
#endif