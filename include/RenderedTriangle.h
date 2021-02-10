#ifndef RENDEREDTRIANGLE_H
#define RENDEREDTRIANGLE_H

#include "RenderedPoint.h"

class RenderedTriangle {
    public:
        RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c, int uvFlag);

        RenderedPoint a = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint b = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint c = RenderedPoint(0, 0, 0, 0, 0);  
};

#endif