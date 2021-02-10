#include "RenderedTriangle.h"

RenderedTriangle::RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c, int uvFlag) : a(_a), b(_b), c(_c) {
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
};