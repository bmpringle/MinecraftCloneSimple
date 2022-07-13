#ifndef RENDEREDPOINT_H
#define RENDEREDPOINT_H

class RenderedPoint {
    public:
        RenderedPoint(float _x, float _y, float _z, float _u = 0, float _v = 0);

        float x = 0;
        float y = 0;
        float z = 0;

        float u = 0;
        float v = 0;

        RenderedPoint operator*(double rhs);

        void rotateX90(double x, double y, double z);

        void rotateY90(double x, double y, double z);

        void rotateZ90(double x, double y, double z);
};
#endif