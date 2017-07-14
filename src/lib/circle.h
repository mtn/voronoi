#ifndef CIRCLE_H
#define CIRCLE_H

#include "line.h"

class Circle {
    public:
        Point* center;
        double radius;

        Circle(Point* center, double radius);

        static bool isVertexEvent(Point* a, Point* b, Point* c);
        static Circle* computeCircumcircle(Point* a, Point* b, Point* c);
};

#endif /* CIRCLE_H */

