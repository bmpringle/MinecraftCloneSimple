#ifndef MODEL_H
#define MODEL_H

struct AABB {
    AABB(float _x, float _y, float _z, float _xs, float _ys, float _zs) : startX(_x), startY(_y), startZ(_z), xSize(_xs), ySize(_ys), zSize(_zs) {

    } 

    float startX = 0;
    float startY = 0;
    float startZ = 0;
    float xSize = 0;
    float ySize = 0;
    float zSize = 0;
};

class Model {
    public:
        virtual AABB getAABB() = 0;
};

#endif