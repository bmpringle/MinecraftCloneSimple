#include "Block.h"
#include "BlockArrayData.h"
#include "BlockData.h"
#include "World.h"
#include "ItemStack.h"

std::string Block::getName() {
    return "block";
}

Block::Block() {

}

AABB Block::getAABB(int data) {
    return AABB(0, 0, 0, 1, 1, 1);
}

BlockRenderedModel Block::getRenderedModel(int data) {
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

    std::array<RenderedTriangle, 2> f1 = {t1, t2};
    BlockFace downFace = BlockFace(f1, DOWN);

    std::array<RenderedTriangle, 2> f2 = {t3, t4};
    BlockFace upFace = BlockFace(f2, UP);

    std::array<RenderedTriangle, 2> f3 = {t5, t6};
    BlockFace westFace = BlockFace(f3, WEST);

    std::array<RenderedTriangle, 2> f4 = {t7, t8};
    BlockFace eastFace = BlockFace(f4, EAST);

    std::array<RenderedTriangle, 2> f5 = {t9, t10};
    BlockFace southFace = BlockFace(f5, SOUTH);

    std::array<RenderedTriangle, 2> f6 = {t11, t12};
    BlockFace northFace = BlockFace(f6, NORTH);

    std::array<BlockFace, 6> blockFaceArray = {upFace, downFace, northFace, southFace, eastFace, westFace};
    return BlockRenderedModel(blockFaceArray);
}

std::string Block::getTextureName(SideEnum side, int data) {
    return "notexture.png";
}

bool Block::isSolid(int data) {
    return true;
}

bool Block::isOpaque(int data) {
    return true;
}

int Block::getXRotation(int data) {
    return 0;
}

int Block::getYRotation(int data) {
    return 0;
}

int Block::getZRotation(int data) {
    return 0;
}

void Block::onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data) {
    
}

void Block::updateBlock(BlockArrayData* data, BlockData* blockToUpdate) {

}

void Block::rotateModel(BlockRenderedModel& model, int data) {
    int xRotation = getXRotation(data) / 90;
    int yRotation = getYRotation(data) / 90;
    int zRotation = getZRotation(data) / 90;
    for(int i = 0; i < xRotation; ++i) {
        model.rotateX90();
    }
    for(int i = 0; i < yRotation; ++i) {
        model.rotateY90();
    }
    for(int i = 0; i < zRotation; ++i) {
        model.rotateZ90();
    }
}

bool Block::onBlockActivated(World* world, BlockPos pos, ItemStack* stack, int* data) {
    return false;
}