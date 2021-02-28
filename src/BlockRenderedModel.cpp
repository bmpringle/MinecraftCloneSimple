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