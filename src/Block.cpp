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
    RenderedPoint p1 = RenderedPoint(0, 0, 0);
    RenderedPoint p2 = RenderedPoint(1, 0, 0);
    RenderedPoint p3 = RenderedPoint(1, 0, 1);
    RenderedPoint p4 = RenderedPoint(0, 0, 1);
    RenderedPoint p5 = RenderedPoint(0, 1, 0);
    RenderedPoint p6 = RenderedPoint(1, 1, 0);
    RenderedPoint p7 = RenderedPoint(1, 1, 1);
    RenderedPoint p8 = RenderedPoint(0, 1, 1);

    RenderedTriangle t1 = RenderedTriangle(p1, p2, p3);
    RenderedTriangle t2 = RenderedTriangle(p1, p4, p3);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p7);
    RenderedTriangle t4 = RenderedTriangle(p5, p8, p7);
    RenderedTriangle t5 = RenderedTriangle(p1, p4, p8);
    RenderedTriangle t6 = RenderedTriangle(p1, p5, p8);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p7);
    RenderedTriangle t8 = RenderedTriangle(p2, p6, p7);
    RenderedTriangle t9 = RenderedTriangle(p1, p2, p5);
    RenderedTriangle t10 = RenderedTriangle(p2, p5, p6);
    RenderedTriangle t11 = RenderedTriangle(p3, p4, p8);
    RenderedTriangle t12 = RenderedTriangle(p3, p7, p8);

    RenderedTriangle triangleArray[12] = {t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12};
    std::vector<RenderedTriangle> triangles = std::vector<RenderedTriangle>();

    for(RenderedTriangle triangle : triangleArray) {
        triangles.push_back(triangle);
    }

    return RenderedModel(triangles);
    

}