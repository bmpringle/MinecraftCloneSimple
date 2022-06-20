#include "BlockDoor.h"
#include "Pos.h"
#include "World.h"

/*
 * metadata format: 
 *  N -> m % 4 = 0, S -> m % 4 = 1, E -> m % 4 = 2, W -> m % 4 = 3
 *  bottom -> m % 8 < 4, top -> m % 8 >= 4
 *  closed -> m % 16 < 8, open -> m % 16 >= 8
 *  left -> m % 32 < 16, right -> m % 32 >= 16
 */

BlockDoor::BlockDoor() {
    numberOfVariants = 32; 
}

std::string BlockDoor::getName() {
    return "door";
}

int serializeMetadata(SideEnum sideFacing, bool isBottomDoorBlock, bool isDoorOpen, bool isRightDoor) {
    int sideFacingMeta = 0;

    switch(sideFacing) {
        case NORTH:
            sideFacingMeta = 0;
            break;
        case SOUTH:
            sideFacingMeta = 1;
            break;
        case EAST:
            sideFacingMeta = 2;
            break;
        case WEST:
            sideFacingMeta = 3;
            break;
        default:
            std::cout << "sideFacing is not N/S/E/W, this should not happen" << std::endl;
            break;
    }
    int rightDoorMeta = ((isRightDoor) ? 1 : 0) * 16; //options * stride of choice
    int doorOpenMeta = ((isDoorOpen) ? 1 : 0) * 8;
    int bottomDoorBlockMeta = ((isBottomDoorBlock) ? 0 : 1) * 4;
    return rightDoorMeta + doorOpenMeta + bottomDoorBlockMeta + sideFacingMeta;
}

SideEnum getSideFromMetadata(int data) {
    switch(data % 4) {
        case 0:
            return NORTH;
        case 1:
            return SOUTH;
        case 2:
            return EAST;
        case 3:
            return WEST;
        default:
            throw std::runtime_error("data % 4 is not 0-3, should not happen");
            break;
    }
}

bool getIsBottomDoorBlockFromMetadata(int data) {
    return (data % 8) < 4;
}

bool getIsDoorOpenFromMetadata(int data) {
    return (data % 16) >= 8;
}

bool getIsRightDoorFromMetadata(int data) {
    return (data % 32) >= 16;
}

int toggleDoorInMetadata(int data) {
    return serializeMetadata(getSideFromMetadata(data), getIsBottomDoorBlockFromMetadata(data), !getIsDoorOpenFromMetadata(data), getIsRightDoorFromMetadata(data));
}

int toggleDoorSideInMetadata(int data) {
    return serializeMetadata(getSideFromMetadata(data), getIsBottomDoorBlockFromMetadata(data), getIsDoorOpenFromMetadata(data), !getIsRightDoorFromMetadata(data));
}

std::string BlockDoor::getTextureName(SideEnum side, int data) {
    bool isBottom = getIsBottomDoorBlockFromMetadata(data);
    return (isBottom) ? "bottom_door.png" : "top_door.png";
}

int BlockDoor::getXRotation(int data) {
    return 0;
}

int BlockDoor::getYRotation(int data) {
    switch(getSideFromMetadata(data)) {
        case NORTH:
            return 180;
        case SOUTH:
            return 0;
        case EAST:
            return 90;
        case WEST:
            return 270; 
        default:
            throw std::runtime_error("getSidefromMetadata somehow didn't return N/S/E/W ...");
    }
}

int BlockDoor::getZRotation(int data) {
    return 0;
}

void BlockDoor::onPlaced(SideEnum hPlacementAngle, SideEnum sideLookingAt, int* data) {  
    *data = serializeMetadata(hPlacementAngle, (*data == 0), false, false);  
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

    t11.a.u = 1 - t11.a.u;
    t11.b.u = 1 - t11.b.u;
    t11.c.u = 1 - t11.c.u;

    t12.a.u = 1 - t12.a.u;
    t12.b.u = 1 - t12.b.u;
    t12.c.u = 1 - t12.c.u;

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

    if(getIsRightDoorFromMetadata(data)) {
        model.rotateY90(0.5, 0, 3.0/32.0);
        model.rotateY90(0.5, 0, 3.0/32.0);

        if(getIsDoorOpenFromMetadata(data)) {
            model.rotateY90(0.5, 0, 3.0/32.0);
            model.rotateY90(0.5, 0, 3.0/32.0);
            model.rotateY90();
            model.rotateY90();
            model.rotateY90();
        }
    }else if(getIsDoorOpenFromMetadata(data)) {
        model.rotateY90(0.5, 0, 3.0/32.0);
        model.rotateY90(0.5, 0, 3.0/32.0);
        model.rotateY90();
    }

    return model;
}

AABB BlockDoor::getAABB(int data) {
    AABB absoluteAABB = AABB(0, 0, 0, 1, 1, 3.0/16.0);
    int xRotation = round((float) getXRotation(data) / 90.0);
    int yRotation = round((float) getYRotation(data) / 90.0);
    int zRotation = round((float) getZRotation(data) / 90.0);

    for(int i = 0; i < xRotation; ++i) {
        absoluteAABB.rotateX90(0.5, 0.5, 0.5);
    }
    for(int i = 0; i < yRotation; ++i) {
        absoluteAABB.rotateY90(0.5, 0.5, 0.5);
    }
    for(int i = 0; i < zRotation; ++i) {
        absoluteAABB.rotateZ90(0.5, 0.5, 0.5);
    }

    if(getIsRightDoorFromMetadata(data)) {
        if(getIsDoorOpenFromMetadata(data)) {
            absoluteAABB.rotateY90(0.5, 0.5, 0.5);
            absoluteAABB.rotateY90(0.5, 0.5, 0.5);
            absoluteAABB.rotateY90(0.5, 0.5, 0.5);
        }
    }else if(getIsDoorOpenFromMetadata(data)) {
        absoluteAABB.rotateY90(0.5, 0.5, 0.5);
    }

    return absoluteAABB;
}

bool BlockDoor::onBlockActivated(World* world, BlockPos pos, ItemStack* stack, int* data) {
    *data = toggleDoorInMetadata(*data);

    if(getIsBottomDoorBlockFromMetadata(*data)) {
        BlockData& blockAbove = world->getBlockData()->getBlockReferenceAtPosition(pos.getAbove());
        if(getIsDoorOpenFromMetadata(blockAbove.getData()) != getIsDoorOpenFromMetadata(*data)) {
            blockAbove.activateBlock(world, stack);
        }
    }else {
        BlockData& blockBelow = world->getBlockData()->getBlockReferenceAtPosition(pos.getBelow());
        if(getIsDoorOpenFromMetadata(blockBelow.getData()) != getIsDoorOpenFromMetadata(*data)) {
            blockBelow.activateBlock(world, stack);
        }
    }

    BlockPos left = BlockPos(0, 0, 0);
    BlockPos right = BlockPos(0, 0, 0);

    switch(getSideFromMetadata(*data)) { //go from absolute l/r to relative l/r
        case NORTH:
            left = pos.getRight();
            right = pos.getLeft();
            break;
        case SOUTH:
            left = pos.getLeft();
            right = pos.getRight();
            break;
        case EAST:
            left = pos.getFront();
            right = pos.getBehind();
            break;
        case WEST:
            left = pos.getBehind();
            right = pos.getFront();
            break;
        default:
            break;
    }

    if(getIsRightDoorFromMetadata(*data)) {
        BlockData& leftBlock = world->getBlockData()->getBlockReferenceAtPosition(left);
        if(getIsDoorOpenFromMetadata(leftBlock.getData()) != getIsDoorOpenFromMetadata(*data)) {
            leftBlock.activateBlock(world, stack);
        }
    }else {
        BlockData& rightBlock = world->getBlockData()->getBlockReferenceAtPosition(right);
        if(rightBlock.getBlockType() == Blocks::door && getIsRightDoorFromMetadata(rightBlock.getData())) {
            if(getIsDoorOpenFromMetadata(rightBlock.getData()) != getIsDoorOpenFromMetadata(*data)) {
                rightBlock.activateBlock(world, stack);
            }
        }
    }
    return true;
}

bool canPairDoorMetadata(int meta1, int meta2) {
    return ((getIsBottomDoorBlockFromMetadata(meta1) == getIsBottomDoorBlockFromMetadata(meta2)) &&
           (getSideFromMetadata(meta1) == getSideFromMetadata(meta2)));
}

void BlockDoor::updateBlock(BlockArrayData* data, BlockData* blockToUpdate) {
    BlockPos pos = blockToUpdate->getPos();

    if(getIsBottomDoorBlockFromMetadata(blockToUpdate->getData())) {
        if(data->getBlockReferenceAtPosition(pos.getAbove()).isBlockAir()) {
            data->removeBlockAtPosition(pos);
        }
    }else {
        if(data->getBlockReferenceAtPosition(pos.getBelow()).isBlockAir()) {
            data->removeBlockAtPosition(pos);
        }
    }

    BlockPos left = BlockPos(0, 0, 0);
    BlockPos twoLeft = BlockPos(0, 0, 0);
    BlockPos right = BlockPos(0, 0, 0);

    switch(getSideFromMetadata(blockToUpdate->getData())) { //go from absolute l/r to relative l/r
        case NORTH:
            left = pos.getRight();
            twoLeft = left.getRight();
            right = pos.getLeft();
            break;
        case SOUTH:
            left = pos.getLeft();
            twoLeft = left.getLeft();
            right = pos.getRight();
            break;
        case EAST:
            left = pos.getFront();
            twoLeft = left.getFront();
            right = pos.getBehind();
            break;
        case WEST:
            left = pos.getBehind();
            twoLeft = left.getBehind();
            right = pos.getFront();
            break;
        default:
            break;
    }

    BlockData leftBlock = data->getBlockReferenceAtPosition(left);
    BlockData twoLeftBlock = data->getBlockReferenceAtPosition(twoLeft);
    BlockData rightBlock = data->getBlockReferenceAtPosition(right);

    if(leftBlock.getBlockType() == Blocks::door && canPairDoorMetadata(leftBlock.getData(), blockToUpdate->getData())) {
        if(twoLeftBlock.getBlockType() != Blocks::door || !canPairDoorMetadata(leftBlock.getData(), blockToUpdate->getData())) {
            if(rightBlock.getBlockType() != Blocks::door || !canPairDoorMetadata(leftBlock.getData(), blockToUpdate->getData())) {
                if(!getIsRightDoorFromMetadata(blockToUpdate->getData())) {
                    blockToUpdate->setData(toggleDoorSideInMetadata(blockToUpdate->getData()));
                }
                
                return;
            }
        }
    }

    if(getIsRightDoorFromMetadata(blockToUpdate->getData())) {
        blockToUpdate->setData(toggleDoorSideInMetadata(blockToUpdate->getData()));
    }
}
