#pragma once

#include <string>

class Line;

class Point {
    public:
        Point(double x, double y);

        double x;
        double y;

        double dist(Line* l);
        bool onBound(const Point* min, const Point* max);

        bool operator<(const Point& p) const;
        bool operator==(const Point& p) const;

        static double dist(const Point* a, const Point* b);
        static Point* midPoint(Point* a, Point* b);
        static double slope(Point* a, Point* b);

        std::string toString() const;

};

// If the y components are equal, default to comparison in x
struct ComparePoint {
    bool operator()(const Point* p1, const Point* p2) const {
        return (p1->y != p2->y)
            ? (p1->y < p2->y ? -1 : 1)
            : (p1->x < p2->x);
    }
};

