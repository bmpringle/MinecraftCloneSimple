#include "RenderedPoint.h"
#include "glmh.h"

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
    /*glm::mat4x4 rotation = glm::mat4x4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::vec4 rotatedPoint = rotation * glm::vec4(x - x_, y - y_, z - z_, 0);
    rotatedPoint = rotatedPoint + glm::vec4(x_, y_, z_, 0);
    x = rotatedPoint.x;
    y = rotatedPoint.y;
    z = rotatedPoint.z;*/
    double xp = x - x_;
    double yp = y - y_;
    double zp = z - z_;

    double xp_ = zp;
    double yp_ = yp;
    double zp_ = -xp;

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