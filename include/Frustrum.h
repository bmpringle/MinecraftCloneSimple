#ifndef FRUSTRUM_H
#define FRUSTRUM_H
class Plane { 
    public:
        Plane(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {

        }

        double a;
        double b;
        double c;
        double d;
};

class Frustrum {
    public:
        Frustrum(Plane left, Plane right, Plane top, Plane bottom, Plane near, Plane far) : left(left), right(right), top(top), bottom(bottom), near(near), far(far) {

        }

        Plane left;
        Plane right;
        Plane top;
        Plane bottom;
        Plane near;
        Plane far;
};
#endif