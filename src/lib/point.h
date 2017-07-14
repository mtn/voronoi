#ifndef POINT_H
#define POINT_H

class Line;

class Point {
    public:
        Point(double x, double y);

        double x;
        double y;

        double dist(Line* l);

        static double dist(Point* a, Point* b);
        static Point* midPoint(Point* a, Point* b);
        static double slope(Point* a, Point* b);

        // returns true is a has a larger y value than b

        const bool operator==(const Point& a);
        const bool operator<(const Point& a);
        /* const bool operator>(const Point& a); */

};


#endif /* POINT_H */

