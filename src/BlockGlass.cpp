#include "BlockGlass.h"

#include "Blocks.h"

BlockGlass::BlockGlass() {

}

std::string BlockGlass::getName() {
    return "glass";
}

std::string BlockGlass::getTextureName(SideEnum side, int data) {
    return "glass.png";
}

bool BlockGlass::isOpaque(int data) {
    return false;
}

void constructQuadWithBounds(std::vector<RenderedTriangle>& triangleVector, RenderedPoint start, RenderedPoint end) {
    RenderedTriangle bottom_left = RenderedTriangle(start, RenderedPoint(end.x, start.y, end.z, end.u, start.v), RenderedPoint(start.x, end.y, start.z, start.u, end.v), -1);
    triangleVector.push_back(bottom_left);

    RenderedTriangle top_right = RenderedTriangle(RenderedPoint(start.x, end.y, start.z, start.u, end.v), RenderedPoint(end.x, start.y, end.z, end.u, start.v), end, -1);
    triangleVector.push_back(top_right);
}

void constructPillarWithBounds(std::vector<RenderedTriangle>& triangleVector, RenderedPoint start, RenderedPoint end) {
    RenderedPoint currentQuadStart = start;
    RenderedPoint currentQuadEnd = end;

    //face 1
    currentQuadStart.x = start.x;
    currentQuadEnd.x = start.x;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);

    currentQuadStart = start;
    currentQuadEnd = end;

    //face 2

    currentQuadStart.x = end.x;
    currentQuadEnd.x = end.x;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);
    
    currentQuadStart = start;
    currentQuadEnd = end;

    //face 3

    currentQuadStart.y = start.y;
    currentQuadEnd.y = start.y;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);
    
    currentQuadStart = start;
    currentQuadEnd = end;

    //face 4

    currentQuadStart.y = end.y;
    currentQuadEnd.y = end.y;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);
    
    currentQuadStart = start;
    currentQuadEnd = end;

    //face 5

    currentQuadStart.z = start.z;
    currentQuadEnd.z = start.z;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);

    //face 6

    currentQuadStart.z = end.z;
    currentQuadEnd.z = end.z;

    constructQuadWithBounds(triangleVector, currentQuadStart, currentQuadEnd);
    
    currentQuadStart = start;
    currentQuadEnd = end;

}

BlockRenderedModel BlockGlass::getRenderedModel(int data) {
    const float ONE_SIXTEENTH = 1.0 / 16.0;
    const float ONE_THIRTYSECOND = 1.0 / 32.0;

    std::vector<RenderedTriangle> compositeVector;

    constructPillarWithBounds(compositeVector, RenderedPoint(0, 0, 0), RenderedPoint(ONE_SIXTEENTH, 1, ONE_SIXTEENTH));
    
    int pillarVectorSize = compositeVector.size();

    constructQuadWithBounds(compositeVector, RenderedPoint(ONE_SIXTEENTH, ONE_SIXTEENTH, ONE_THIRTYSECOND, ONE_SIXTEENTH, ONE_SIXTEENTH), RenderedPoint(1 - ONE_SIXTEENTH, 1 - ONE_SIXTEENTH, ONE_THIRTYSECOND, 1 - ONE_SIXTEENTH, 1 - ONE_SIXTEENTH));

    BlockFace southFace = BlockFace(compositeVector, SOUTH);

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateY90(0.5, 0.5, 0.5);
    }

    BlockFace westFace = BlockFace(compositeVector, WEST);

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateY90(0.5, 0.5, 0.5);
    }

    BlockFace northFace = BlockFace(compositeVector, NORTH);

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateY90(0.5, 0.5, 0.5);
    }

    BlockFace eastFace = BlockFace(compositeVector, EAST);

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateY90(0.5, 0.5, 0.5);
    }

    for(int i = 0; i < pillarVectorSize; ++i) {
        RenderedTriangle triangle = compositeVector[i];
        triangle.rotateZ90(0.5, 0.5, 0.5);
        compositeVector.push_back(triangle);
        triangle.rotateZ90(0.5, 0.5, 0.5);
        compositeVector.push_back(triangle);
        triangle.rotateZ90(0.5, 0.5, 0.5);
        compositeVector.push_back(triangle);
    }

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateX90(0.5, 0.5, 0.5);
    }

    BlockFace downFace = BlockFace(compositeVector, DOWN);

    for(RenderedTriangle& triangle : compositeVector) {
        triangle.rotateX90(0.5, 0.5, 0.5);
        triangle.rotateX90(0.5, 0.5, 0.5);
    }

    BlockFace upFace = BlockFace(compositeVector, UP);

    std::array<BlockFace, 6> blockFaceArray = {upFace, downFace, northFace, southFace, eastFace, westFace};
    return BlockRenderedModel(blockFaceArray);

}