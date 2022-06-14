#ifndef AABB_H
#define AABB_H

#include "WireframeVertex.h"

#include <vector>

#include "glmh.h"

class BlockPos;
class Pos;

class AABB {

    public:
        AABB(float _x, float _y, float _z, float _xs, float _ys, float _zs) : startX(_x), startY(_y), startZ(_z), xSize(_xs), ySize(_ys), zSize(_zs) {

        } 

        float startX = 0;
        float startY = 0;
        float startZ = 0;
        float xSize = 0;
        float ySize = 0;
        float zSize = 0;

        void add(Pos p);

        void add(BlockPos p);

        void rotateX90();

        void rotateY90();

        void rotateZ90();

        float getWidth();

        float getHeight();

        float getDepth();

        std::vector<WireframeVertex> getWireframeFromAABB();
};

#endif