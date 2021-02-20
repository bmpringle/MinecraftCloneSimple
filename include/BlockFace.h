#ifndef BLOCKFACE_H
#define BLOCKFACE_H
#include "RenderedTriangle.h"
#include <array>
#include "SideEnum.h"

class BlockFace {
    public:
        BlockFace(std::array<RenderedTriangle, 2> tris, SideEnum s);

        std::array<RenderedTriangle, 2> triangles;
        SideEnum side;
};
#endif