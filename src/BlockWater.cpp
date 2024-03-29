#include "BlockWater.h"

BlockWater::BlockWater() {
    aabb = AABB(0, 0, 0, 1, 0.9, 1);
}

std::string BlockWater::getTextureName(SideEnum side, int data = 0) {
    return "water.png";
}

std::string BlockWater::getName() {
    return "water";
}

bool BlockWater::isSolid(int data = 0) {
    return false;
}

bool BlockWater::isOpaque(int data = 0) {
    return false;
}

bool BlockWater::isLiquid(int data = 0) {
    return true;
}

BlockRenderedModel BlockWater::getRenderedModel(int data = 0) {
    RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p2 = RenderedPoint(1, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p3 = RenderedPoint(1, 0, 1, /**uv coords*/ 0, 0);
    RenderedPoint p4 = RenderedPoint(0, 0, 1, /**uv coords*/ 0, 0);
    RenderedPoint p5 = RenderedPoint(0, 0.9, 0, /**uv coords*/ 0, 0);
    RenderedPoint p6 = RenderedPoint(1, 0.9, 0, /**uv coords*/ 0, 0);
    RenderedPoint p7 = RenderedPoint(1, 0.9, 1, /**uv coords*/ 0, 0);
    RenderedPoint p8 = RenderedPoint(0, 0.9, 1, /**uv coords*/ 0, 0);

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

    std::vector<RenderedTriangle> f1 = {t1, t2};
    BlockFace downFace = BlockFace(f1, DOWN);

    std::vector<RenderedTriangle> f2 = {t3, t4};
    BlockFace upFace = BlockFace(f2, UP);

    std::vector<RenderedTriangle> f3 = {t5, t6};
    BlockFace westFace = BlockFace(f3, WEST);

    std::vector<RenderedTriangle> f4 = {t7, t8};
    BlockFace eastFace = BlockFace(f4, EAST);

    std::vector<RenderedTriangle> f5 = {t9, t10};
    BlockFace southFace = BlockFace(f5, SOUTH);

    std::vector<RenderedTriangle> f6 = {t11, t12};
    BlockFace northFace = BlockFace(f6, NORTH);

    std::array<BlockFace, 6> blockFaceArray = {upFace, downFace, northFace, southFace, eastFace, westFace};
    return BlockRenderedModel(blockFaceArray);
}
