#include "BlockDoor.h"
#include "Pos.h"
#include "World.h"

BlockDoor::BlockDoor() {
    numberOfVariants = 16;
}

std::string BlockDoor::getName() {
    return "door";
}

std::string BlockDoor::getTextureName(SideEnum side, int data) {
    bool isBottom = false;
    if(data < 8) {
        isBottom = true;
    }
    return (isBottom) ? "bottom_door.png" : "top_door.png";
}

int BlockDoor::getXRotation(int data) {
    return 0;
}

int BlockDoor::getYRotation(int data) {
    int NSEWData = data % 4;
    switch(NSEWData) {
        case 0:
            return 180;
        case 1:
            return 0;
        case 2:
            return 90;
        case 3:
            return 270; 
    }
    return 0;
}

int BlockDoor::getZRotation(int data) {
    return 0;
}

void BlockDoor::onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data) {    
    bool isBottom = true;
    if(*data != 0) {
        isBottom = false;
    }
    switch(hPlacementAngle) {
        case NORTH:
            *data = 0 + ((!isBottom) ? 8 : 0);
            return;
        case SOUTH:
            *data = 1 + ((!isBottom) ? 8 : 0);
            return;
        case EAST:
            *data = 2 + ((!isBottom) ? 8 : 0);
            return;
        case WEST:
            *data = 3 + ((!isBottom) ? 8 : 0);
            return;
        default:
            break;
    }
    return;
}

BlockRenderedModel BlockDoor::getRenderedModel(int data = 0) {
    RenderedPoint p1 = RenderedPoint(0, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p2 = RenderedPoint(1, 0, 0, /**uv coords*/ 0, 0);
    RenderedPoint p3 = RenderedPoint(1, 0, 3.0/16.0, /**uv coords*/ 0, 0);
    RenderedPoint p4 = RenderedPoint(0, 0, 3.0/16.0, /**uv coords*/ 0, 0);
    RenderedPoint p5 = RenderedPoint(0, 1, 0, /**uv coords*/ 0, 0);
    RenderedPoint p6 = RenderedPoint(1, 1, 0, /**uv coords*/ 0, 0);
    RenderedPoint p7 = RenderedPoint(1, 1, 3.0/16.0, /**uv coords*/ 0, 0);
    RenderedPoint p8 = RenderedPoint(0, 1, 3.0/16.0, /**uv coords*/ 0, 0);

    RenderedTriangle t1 = RenderedTriangle(p4, p2, p1, 1, 3.0/16.0);
    RenderedTriangle t2 = RenderedTriangle(p3, p2, p4, 0, 3.0/16.0);
    RenderedTriangle t3 = RenderedTriangle(p5, p6, p8, 0, 1, 3.0/16.0);
    RenderedTriangle t4 = RenderedTriangle(p8, p6, p7, 1, 1, 3.0/16.0);
    RenderedTriangle t5 = RenderedTriangle(p4, p1, p8, 0, 3.0/16.0);
    RenderedTriangle t6 = RenderedTriangle(p8, p1, p5, 1, 3.0/16.0);
    RenderedTriangle t7 = RenderedTriangle(p2, p3, p6, 0, 3.0/16.0);
    RenderedTriangle t8 = RenderedTriangle(p6, p3, p7, 1, 3.0/16.0);
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

    BlockRenderedModel model = BlockRenderedModel(blockFaceArray);

    bool isOpen = (data % 8) > 3;
    if(isOpen) {
        model.rotateY90();
    }
    return model;
}

AABB BlockDoor::getAABB(int data) {
    AABB absoluteAABB = AABB(0, 0, 0, 1, 1, 3.0/16.0);
    absoluteAABB.add(Pos(-0.5, -0.5, -0.5));
    int xRotation = getXRotation(data) / 90;
    int yRotation = getYRotation(data) / 90;
    int zRotation = getZRotation(data) / 90;
    for(int i = 0; i < xRotation; ++i) {
        absoluteAABB.rotateX90();
    }
    for(int i = 0; i < yRotation; ++i) {
        absoluteAABB.rotateY90();
    }
    for(int i = 0; i < zRotation; ++i) {
        absoluteAABB.rotateZ90();
    }
    bool isOpen = (data % 8) > 3;
    if(isOpen) {
        absoluteAABB.rotateY90();
    }

    absoluteAABB.add(Pos(0.5, 0.5, 0.5));

    return absoluteAABB;
}

bool BlockDoor::onBlockActivated(World* world, BlockPos pos, ItemStack* stack, int* data) {
    bool isOpen = (*data % 8) > 3;
    if(isOpen) {
        *data = *data - 4;
    }else {
        *data = *data + 4;
    }

    if(*data < 8) {
        if(world->getBlockData()->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y + 1, pos.z)).getData() % 8 != *data % 8) {
            world->getBlockData()->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y + 1, pos.z)).activateBlock(world, stack);
        }
    }else {
        if(world->getBlockData()->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y - 1, pos.z)).getData() % 8 != *data % 8) {
            world->getBlockData()->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y - 1, pos.z)).activateBlock(world, stack);
        }
    }
    return true;
}

void BlockDoor::updateBlock(BlockArrayData* data, BlockData* blockToUpdate) {
    BlockPos pos = blockToUpdate->getPos();

    if(blockToUpdate->getData() < 8) {
        if(data->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y + 1, pos.z)).getBlockType() == nullptr) {
            data->removeBlockAtPosition(pos);
        }
    }else {
        if(data->getBlockReferenceAtPosition(BlockPos(pos.x, pos.y - 1, pos.z)).getBlockType() == nullptr) {
            data->removeBlockAtPosition(pos);
        }
    }
}
