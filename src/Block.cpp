#include "Block.h"

std::string Block::getName() {
    return "block";
}

BlockPos Block::getPos() {
    return pos;
}

void Block::setPos(BlockPos pos_) {
    pos = pos_;
}

Block::Block() : pos(BlockPos(0, 0, 0)) {

}

AABB Block::getAABB() {
    return AABB(0, 0, 0, 1, 1, 1);
}

RenderedModel Block::getRenderedModel() {
    RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p2 = RenderedPoint(1, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p3 = RenderedPoint(1, 0, 1, /**uv coords*/ 0, 0);
    RenderedPoint p4 = RenderedPoint(0, 0, 1, /**uv coords*/ 0, 0);
    RenderedPoint p5 = RenderedPoint(0, 1, 0, /**uv coords*/ 0, 0);
    RenderedPoint p6 = RenderedPoint(1, 1, 0, /**uv coords*/ 0, 0);
    RenderedPoint p7 = RenderedPoint(1, 1, 1, /**uv coords*/ 0, 0);
    RenderedPoint p8 = RenderedPoint(0, 1, 1, /**uv coords*/ 0, 0);

    RenderedTriangle t1 = RenderedTriangle(p4, p2, p1, 1);
    RenderedTriangle t2 = RenderedTriangle(p3, p2, p4, 0);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p8, 0);
    RenderedTriangle t4 = RenderedTriangle(p8, p6, p7, 1);
    RenderedTriangle t5 = RenderedTriangle(p4, p1, p8, 0);
    RenderedTriangle t6 = RenderedTriangle(p8, p1, p5, 1);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p6, 0);
    RenderedTriangle t8 = RenderedTriangle(p6, p3, p7, 1);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5, 0);
    RenderedTriangle t10 = RenderedTriangle(p5, p2, p6, 1);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p7, 0);
    RenderedTriangle t12 = RenderedTriangle(p7, p4, p8, 1);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }

    return RenderedModel(triangles);
}

std::string Block::getTextureName() {
    return "notexture.png";
}