#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>


double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->pe->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


// Analytically, this reduces to problem of finding center of circle defined by 2 points
// and a tangent line. However, the resulting quadratic system of equations is difficult
// to resolve programatically. Rather, I find the parabolas defined by each focus-and-directrix
// and solve a quadratic equation to compute their intersection.
// Note that each point in the breakpoint pair is the focus of a parabola.
double computeLeftIntersection(Breakpoint& b, double y) {
    double a1 = 1.0 / (2 * (b.first->y - y));
    double b1 = b.first->x / (b.first->y - y);
    double c1 = (pow(b.first->x,2)  + pow(b.first->y,2)  + pow(y,2 ))/ ((b.first->y - y) * 2);

    double a2 = 1.0 / (2 * (b.second->y - y));
    double b2 = b.second->x / (b.second->y - y);
    double c2 = (pow(b.second->x,2)  + pow(b.second->y,2)  + pow(y,2 ))/ ((b.second->y - y) * 2);

    double A = a1-a2;
    double B = b1-b2;
    double C = c1-c2;

    // Because we know there's an intersection, no need to check for degenerate case
    return (-B + sqrt(pow(B,2) - 4 * A * C))/ (2 * A);
}

double computeRightIntersection(Breakpoint& b, double y) {
    double a1 = 1.0 / (2 * (b.first->y - y));
    double b1 = b.first->x / (b.first->y - y);
    double c1 = (pow(b.first->x,2)  + pow(b.first->y,2)  + pow(y,2 ))/ ((b.first->y - y) * 2);

    double a2 = 1.0 / (2 * (b.second->y - y));
    double b2 = b.second->x / (b.second->y - y);
    double c2 = (pow(b.second->x,2)  + pow(b.second->y,2)  + pow(y,2 ))/ ((b.second->y - y) * 2);

    double A = a1-a2;
    double B = b1-b2;
    double C = c1-c2;

    // Because we know there's an intersection, no need to check for degenerate case
    return (-B - sqrt(pow(B,2) - 4 * A * C))/ (2 * A);
}

