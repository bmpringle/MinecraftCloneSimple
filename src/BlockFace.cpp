#include "BlockFace.h"

BlockFace::BlockFace(std::array<RenderedTriangle, 2> tris, SideEnum s) : triangles(tris), side(s) {

}

void BlockFace::rotateX90(double x, double y, double z) {
    triangles[0].rotateX90(x, y, z);
    triangles[1].rotateX90(x, y, z);
}

void BlockFace::rotateY90(double x, double y, double z) {
    triangles[0].rotateY90(x, y, z);
    triangles[1].rotateY90(x, y, z);
}

void BlockFace::rotateZ90(double x, double y, double z) {
    triangles[0].rotateZ90(x, y, z);
    triangles[1].rotateZ90(x, y, z);
}