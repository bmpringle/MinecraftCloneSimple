#include "RenderedTriangle.h"

RenderedTriangle::RenderedTriangle() {

}

RenderedTriangle::RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c, int uvFlag, double scaleX, double scaleY) : a(_a), b(_b), c(_c) {
    if(uvFlag == 0) {
        a.u = 0;
        a.v = 0;
        
        b.u = 1 * scaleX;
        b.v = 0;

        c.u = 0;
        c.v = 1 * scaleY;
    }else if(uvFlag == 1){
        a.u = 0;
        a.v = 1 * scaleY;
        
        b.u = 1 * scaleX;
        b.v = 0;

        c.u = 1 * scaleX;
        c.v = 1 * scaleY;                
    }
};

void RenderedTriangle::rotateX90(double x, double y, double z) {
    a.rotateX90(x, y, z);
    b.rotateX90(x, y, z);
    c.rotateX90(x, y, z);
}

void RenderedTriangle::rotateY90(double x, double y, double z) {
    a.rotateY90(x, y, z);
    b.rotateY90(x, y, z);
    c.rotateY90(x, y, z);
}

void RenderedTriangle::rotateZ90(double x, double y, double z) {
    a.rotateZ90(x, y, z);
    b.rotateZ90(x, y, z);
    c.rotateZ90(x, y, z);
}