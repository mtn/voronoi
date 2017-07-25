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
double computeIntersection(Breakpoint& b, double y) {

}

