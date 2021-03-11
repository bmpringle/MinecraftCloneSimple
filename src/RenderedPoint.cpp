#include "RenderedPoint.h"

RenderedPoint::RenderedPoint(float _x, float _y, float _z, float _u, float _v) : x(_x), y(_y), z(_z), u(_u), v(_v) {

}

RenderedPoint RenderedPoint::operator*(double rhs) {
    return RenderedPoint(x * rhs, y * rhs, z * rhs, u, v);
}

void RenderedPoint::rotateX90(double x_, double y_, double z_) {
    double xp = x - x_;
    double yp = y - y_;
    double zp = z - z_;

    double xp_ = xp;
    double yp_ = zp;
    double zp_ = -yp;

    x = xp_ + x_;
    y = yp_ + y_;
    z = zp_ + z_; 
}

void RenderedPoint::rotateY90(double x_, double y_, double z_) {
    double xp = x - x_;
    double yp = y - y_;
    double zp = z - z_;

    double xp_ = -zp;
    double yp_ = yp;
    double zp_ = xp;

    x = xp_ + x_;
    y = yp_ + y_;
    z = zp_ + z_; 
}

void RenderedPoint::rotateZ90(double x_, double y_, double z_) {
    double xp = x - x_;
    double yp = y - y_;
    double zp = z - z_;

    double xp_ = -yp;
    double yp_ = xp;
    double zp_ = zp;

    x = xp_ + x_;
    y = yp_ + y_;
    z = zp_ + z_; 
}