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
double computeIntersection(Breakpoint& b, double sweeplineY) {
    double ay = std::get<0>(b)->y - sweeplineY;
    double bx = std::get<1>(b)->x - std::get<0>(b)->x;
    double by = std::get<1>(b)->y - sweeplineY;

    if(ay == by) {
        return (std::get<0>(b)->x + std::get<1>(b)->x) / 2;
    }

    double shiftedIntersect = (ay*bx - sqrt(ay*by*(pow(ay-by,2) + pow(bx,2))))/(ay-by);

    return shiftedIntersect + std::get<0>(b)->x;
}


