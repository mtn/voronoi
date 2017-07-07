#include "lib/point.h"
#include "lib/line.h"

#include <cmath>
#include <cassert>

double Point::dist(Point a) {
    return sqrt(pow(a.x-this->x,2) + pow(a.y-this->y,2));
}

double Point::dist(Line* l) {
    assert(l->slope == 0);
    return std::abs(this->y - l->yIntercept);
}

Point* Point::midPoint(Point a) {
    return new Point((a.x + this->x) / 2, (a.y + this->y) / 2);
}

Point* Point::midPoint(Point a, Point b) {
    return new Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

