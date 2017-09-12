#include "lib/line.h"

#include <cmath>
#include <cassert>


Line::Line(Point* a, Point* b) {
    slope = Point::slope(a,b);
    yIntercept = a->y - slope * a->x;
}

Line::Line(double slope, double intercept) {
    this->slope = slope;
    this->yIntercept = intercept;
}

Line::Line(Point* a, double slope) {
    this->slope = slope;
    this->yIntercept = -a->x * slope + a->y;
}

double Line::getY(double x) {
    return this->slope * x + this->yIntercept;
}

double Line::getX(double y) {
    return (y - this->yIntercept) / this->slope;
}

Point* Line::getIntersection(Line* l1, Line* l2) {
    double x = (l2->yIntercept - l1->yIntercept) / (l2->slope - l1->slope);
    return new Point(x,l1->getY(x));
}
//
// WARNING: Crashes on non-horizontal input.
double Line::dist(Point* p) {
    assert(this->slope == 0);
    return std::abs(p->y - this->yIntercept);
}



