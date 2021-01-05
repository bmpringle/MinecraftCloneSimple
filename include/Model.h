#ifndef MODEL_H
#define MODEL_H

#include <vector>

class BlockPos;
class Pos;

struct AABB {
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
};

class RenderedPoint {
    public:
        RenderedPoint(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {

        }

        float x = 0;
        float y = 0;
        float z = 0;

        float u = 0;
        float v = 0;
};

class RenderedTriangle {
    public:
        RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c, int uvFlag) : a(_a), b(_b), c(_c) {
            if(uvFlag == 0) {
                a.u = 0;
                a.v = 0;
                
                b.u = 1;
                b.v = 0;

                c.u = 0;
                c.v = 1;
            }else if(uvFlag == 1){
                a.u = 0;
                a.v = 1;
                
                b.u = 1;
                b.v = 0;

                c.u = 1;
                c.v = 1;                
            }
        }

        RenderedPoint a = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint b = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint c = RenderedPoint(0, 0, 0, 0, 0);  
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