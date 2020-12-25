#ifndef MODEL_H
#define MODEL_H

#include <vector>

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

class RenderedPoint {
    public:
        RenderedPoint(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

        }

        float x = 0;
        float y = 0;
        float z = 0;
};

class RenderedTriangle {
    public:
        RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c) : a(_a), b(_b), c(_c) {

        }

        RenderedPoint a = RenderedPoint(0, 0, 0);
        RenderedPoint b = RenderedPoint(0, 0, 0);
        RenderedPoint c = RenderedPoint(0, 0, 0);  
};

class RenderedModel {
    public:
        RenderedModel(std::vector<RenderedTriangle> _renderedModel) : renderedModel(_renderedModel) {

        }

        std::vector<RenderedTriangle> renderedModel = std::vector<RenderedTriangle>();
};

class Model {
    public:
        virtual AABB getAABB() = 0;

        virtual RenderedModel getRenderedModel() = 0;
};

#endif