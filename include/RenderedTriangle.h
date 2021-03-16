#ifndef RENDEREDTRIANGLE_H
#define RENDEREDTRIANGLE_H

#include "RenderedPoint.h"

class RenderedTriangle {
    public:
        RenderedTriangle(RenderedPoint _a, RenderedPoint _b, RenderedPoint _c, int uvFlag, double scaleX = 1, double scaleY = 1);

        void rotateX90(double x, double y, double z);

        void rotateY90(double x, double y, double z);

        void rotateZ90(double x, double y, double z);

        RenderedPoint a = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint b = RenderedPoint(0, 0, 0, 0, 0);
        RenderedPoint c = RenderedPoint(0, 0, 0, 0, 0);  
};

#endif