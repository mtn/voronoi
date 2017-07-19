#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <limits>


double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->pe->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


// Each point is the focus of a parabola, and the directrix is shared
double computeIntersection(Breakpoint b, double y) {


    return 0;
}



