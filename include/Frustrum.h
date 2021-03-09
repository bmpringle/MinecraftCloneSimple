#ifndef FRUSTRUM_H
#define FRUSTRUM_H
class Plane { 
    public:
        Plane(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {
            normalCalculate();
        }
        
        Plane() {

        }

        void normalCalculate() {
            normal = glm::vec3(a/sqrt(a*a+b*b+c*c), b/sqrt(a*a+b*b+c*c), c/sqrt(a*a+b*b+c*c));
        }

        double a = 0;
        double b = 0;
        double c = 0;
        double d = 0;

        glm::vec3 normal = glm::vec3(0, 0, 0);
};

class Frustrum {
    public:
        Frustrum(Plane left, Plane right, Plane top, Plane bottom, Plane near, Plane far) : left(left), right(right), top(top), bottom(bottom), near(near), far(far) {

        }

        Frustrum() {

        }

        Plane left = Plane();
        Plane right = Plane();
        Plane top = Plane();
        Plane bottom = Plane();
        Plane near = Plane();
        Plane far = Plane();

        Plane& operator[](int index) {
            switch(index) {
                case 0:
                    return left;
                case 1:
                    return right;
                case 2:
                    return top;
                case 3:
                    return bottom;
                case 4:
                    return near;
                case 5:
                    return far;
                default:
                    throw std::out_of_range("bad");
            }
        }
};
#endif