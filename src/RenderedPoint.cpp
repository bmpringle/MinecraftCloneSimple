#include "RenderedPoint.h"

RenderedPoint::RenderedPoint(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {

}

RenderedPoint RenderedPoint::operator*(double rhs) {
    return RenderedPoint(x * rhs, y * rhs, z * rhs, u, v);
}