#ifndef CIRCLE_H
#define CIRCLE_H

#include "point.h"

const float PI = 3.14159;

class Circle {
    public:
        Point center;
        double radius;

        bool isVertexEvent(Point a, Point b, Point c);

    private:
        Circle* computeCircumcicle(Point a, Point b, Point c);
};

#endif /* CIRCLE_H */

