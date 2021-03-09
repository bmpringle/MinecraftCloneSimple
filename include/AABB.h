#ifndef AABB_H
#define AABB_H

#include "glmh.h"
#include <array>


class BlockPos;
class Pos;

class AABB {

    public:
        AABB(float _x, float _y, float _z, float _xs, float _ys, float _zs) : startX(_x), startY(_y), startZ(_z), xSize(_xs), ySize(_ys), zSize(_zs) {

        } 
        
        std::array<glm::vec3, 8> getPoints() {
            glm::vec3 p1 = glm::vec3(startX, startY, startZ);
            glm::vec3 p2 = glm::vec3(startX + xSize, startY, startZ);
            glm::vec3 p3 = glm::vec3(startX, startY + ySize, startZ);
            glm::vec3 p4 = glm::vec3(startX, startY, startZ + zSize);
            glm::vec3 p5 = glm::vec3(startX + xSize, startY + ySize, startZ);
            glm::vec3 p6 = glm::vec3(startX + xSize, startY, startZ + zSize);
            glm::vec3 p7 = glm::vec3(startX + xSize, startY + ySize, startZ + zSize);
            glm::vec3 p8 = glm::vec3(startX, startY + ySize, startZ + zSize);
            return {p1, p2, p3, p4, p5, p6, p7, p8};
        }

        float startX = 0;
        float startY = 0;
        float startZ = 0;
        float xSize = 0;
        float ySize = 0;
        float zSize = 0;

        void add(Pos p);

        void add(BlockPos p);
};

#endif