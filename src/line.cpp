#include "lib/line.h"
#include "lib/point.h"

Line::Line(Point* a, Point* b) {
    slope = Point::slope(a,b);
    yIntercept = a->y - slope * a->x;
}

Line::Line(double slope, double intercept) {
    this->slope = slope;
    this->yIntercept = intercept;
}

