#include "lib/circle.h"

Circle::Circle(Point* center, double radius) {
    this->center = center;
    this->radius = radius;
}

Circle* computeCircumcircle(Point* a, Point* b, Point* c) {
    Point* midptAB = Point::midPoint(a,b);
    double slopeAB = Point::slope(a,b);
    double slopeBisAB = -1/slopeAB;

    Line* abcdBisector = new Line::Line(midptAB,slopeBisAB);
    return new Circle(new Point(1,2),2);
}


