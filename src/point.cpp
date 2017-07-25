#include "lib/point.h"

#include <cmath>
#include <iostream>


Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

double Point::dist(Point* a, Point* b) {
    return sqrt(pow(a->x-b->x,2) + pow(a->y-b->y,2));
}

Point* Point::midPoint(Point* a, Point* b) {
    return new Point((a->x + b->x) / 2, (a->y + b->y) / 2);
}

double Point::slope(Point* a, Point* b) {
    if(a->x > b->x) {
        return (a->y - b->y) / (a->x - b->x);
    } else {
        return (b->y - a->y) / (b->x - a->x);
    }
}

bool Point::operator<(const Point& p) const {
    return (this->y < p.y) || ((this->y  == p.y) && (this->x < p.x));
}

bool Point::operator==(const Point& p) const {
    return (this->x == p.x) && (this->y == p.y);
}

bool Point::onBound(const Point* min, const Point* max) {
    return this->x == min->x || this->x == max->x
        || this->y == min->y || this->y == max->y;
}

