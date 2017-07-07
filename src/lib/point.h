#ifndef POINT_H
#define POINT_H

class Line;

class Point {
    public:
        Point(double x, double y);

        double x;
        double y;

        double dist(Point* a, Point* b);
        double dist(Line* l);

        static Point* midPoint(Point* a, Point* b);
        static double slope(Point* a, Point* b);
};

#endif /* POINT_H */

