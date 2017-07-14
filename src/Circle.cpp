#include "lib/circle.h"

#include <cstdbool>
#include <cstddef>
#include <iostream>

Circle::Circle(Point* center, double radius) {
    this->center = center;
    this->radius = radius;
}

/* Circle* computeCircumcircle(Point* a, Point* b, Point* c) { */

/*     Point* midptAB = Point::midPoint(a,b); */
/*     double slopeAB = Point::slope(a,b); */
/*     double slopeBisAB = -1/slopeAB; */
/*     Line* abBisector = new Line(midptAB,slopeBisAB); */

/*     Point* midptAC = Point::midPoint(a,b); */
/*     double slopeAC = Point::slope(a,b); */
/*     double slopeBisAC = -1/slopeAC; */
/*     Line* acBisector = new Line(midptAC,slopeBisAC); */

/*     Point* center = Line::getIntersection(abBisector,acBisector); */
/*     return new Circle(center,Point::dist(center,a)); */
/* } */

Circle* Circle::computeCircumcircle(Point* a, Point* b, Point* c) {
    Point* ba = new Point(b->x-a->x,b->y-a->y);
    Point* ca = new Point(c->x-a->x,c->y-a->y);
    double baLength = (ba->x * ba->x) + (ba->y * ba->y);
    double caLength = (ca->x * ca->x) + (ca->y * ca->y);
    double denominator = 2 * (ba->x * ca->y - ba->y * ca->x);
     // Equals 0 for colinear points.  Less than zero if points are ccw and arc is diverging.
    if (denominator <= 0) {
        std::cout << "returning null " << std::endl;
        return nullptr;  // Don't use this circle event!
    };
    double x = a->x + (ca->y * baLength - ba->y * caLength) / denominator ;
    double y = a->y + (ba->x * caLength - ca->x * baLength) / denominator ;
    Point* center = new Point(x,y);
    return new Circle(center,Point::dist(center,a));
}

bool isVertexEvent(Point* a, Point* b, Point* c) {
    /* Circle* d = computeCircumcircle(a,b,c); */

    return true;
}

