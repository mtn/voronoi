#include "lib/circle.h"

Circle::Circle(Point* center, double radius) {
    this->center = center;
    this->radius = radius;
}

Circle* computeCircumcircle(Point* a, Point* b, Point* c) {

    Point* midptAB = Point::midPoint(a,b);
    double slopeAB = Point::slope(a,b);
    double slopeBisAB = -1/slopeAB;
    Line* abBisector = new Line(midptAB,slopeBisAB);

    Point* midptAC = Point::midPoint(a,b);
    double slopeAC = Point::slope(a,b);
    double slopeBisAC = -1/slopeAC;
    Line* acBisector = new Line(midptAC,slopeBisAC);

    Point* center = Line::getIntersection(abBisector,acBisector);
    return new Circle(center,Point::dist(center,a));
}

bool isVertexEvent(Point* a, Point* b, Point* c) {
    /* Circle* d = computeCircumcircle(a,b,c); */

    return true;
}

