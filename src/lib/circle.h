#pragma once

#include "line.h"

class Circle {
    public:
        Point* center;
        double radius;

        Circle(Point* center, double radius);

        static Circle* computeCircumcircle(const Point* a, const Point* b, const Point* c);
};

