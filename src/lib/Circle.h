#ifndef CIRCLE_H
#define CIRCLE_H

#include "line.h"

const float PI = 3.14159;

class Circle {
    public:
        Point* center;
        double radius;

        Circle(Point* center, double radius);

        bool isVertexEvent(Point a, Point b, Point c);

    private:
        Circle* computeCircumcircle(Point* a, Point* b, Point* c);
};

#endif /* CIRCLE_H */

