#include "BlockRenderedModel.h"
#include "RenderedModel.h"

BlockRenderedModel::BlockRenderedModel(std::array<BlockFace, 6> _renderedBlockModel) : renderedBlockModel(_renderedBlockModel) {

}

RenderedModel BlockRenderedModel::toRenderedModel() {
    std::vector<RenderedTriangle> tris = std::vector<RenderedTriangle>();
    for(BlockFace& face : renderedBlockModel) {
        tris.push_back(face.triangles[0]);
        tris.push_back(face.triangles[1]);
    }
    return RenderedModel(tris);
}

void BlockRenderedModel::rotateX90() {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateX90(0.5, 0.5, 0.5);
    }
}

void BlockRenderedModel::rotateY90() {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateY90(0.5, 0.5, 0.5);
    }
}

void BlockRenderedModel::rotateZ90() {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateZ90(0.5, 0.5, 0.5);
    }
}

void BlockRenderedModel::rotateX90(double x, double y, double z) {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateX90(x, y, z);
    }
}

void BlockRenderedModel::rotateY90(double x, double y, double z) {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateY90(x, y, z);
    }
}

void BlockRenderedModel::rotateZ90(double x, double y, double z) {
    for(BlockFace& face : renderedBlockModel) {
        face.rotateZ90(x, y, z);
    }
}