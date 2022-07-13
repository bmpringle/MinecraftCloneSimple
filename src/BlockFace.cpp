#include "BlockFace.h"

BlockFace::BlockFace(std::vector<RenderedTriangle> tris, SideEnum s) : triangles(tris), side(s) {

}

void BlockFace::rotateX90(double x, double y, double z) {
    triangles.at(0).rotateX90(x, y, z);
    triangles.at(1).rotateX90(x, y, z);
}

void BlockFace::rotateY90(double x, double y, double z) {
    triangles.at(0).rotateY90(x, y, z);
    triangles.at(1).rotateY90(x, y, z);
}

void BlockFace::rotateZ90(double x, double y, double z) {
    triangles.at(0).rotateZ90(x, y, z);
    triangles.at(1).rotateZ90(x, y, z);
}