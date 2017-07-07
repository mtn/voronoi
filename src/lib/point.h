#ifndef POINT_H
#define POINT_H

#include "line.h"

class Point {
    public:
        Point(double x, double y);

        double x;
        double y;

        double dist(Point a);
        double dist(Line* l);

        Point* midPoint(Point a);
        Point* midPoint(Point a, Point b);

        static double slope(Point* a, Point* b);

    private:
        int x1;
};

#endif /* POINT_H */

