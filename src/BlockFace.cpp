#include "BlockFace.h"

BlockFace::BlockFace(std::array<RenderedTriangle, 2> tris, SideEnum s) : triangles(tris), side(s) {

}