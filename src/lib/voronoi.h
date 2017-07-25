// Defines internal representation of Voronoi Diagram

#ifndef VORONOI_H
#define VORONOI_H

#include "point.h"
#include "circle.h"

#include <utility>

enum EventType       { CircleE, PointE };
enum CompareResult   { GreaterThan, EqualTo, LessThan };


typedef struct {
    Point* p1;
    Point* p2;
    Point* p3;

    Circle* c;
} CircleEvent;

typedef Point PointEvent;

typedef struct {
    EventType type;

    // One of these will be null
    CircleEvent* ce;
    PointEvent* pe;
} Event;

// Event priority is simply a y value, determining when an event
// comes off the queue
double getEventPriority(const Event* e);
struct CompareEvent {
    bool operator()(const Event* e1, const Event* e2) const {
        double e1Y = getEventPriority(e1);
        double e2Y = getEventPriority(e2);
        return e1Y > e2Y;
    }
};


// Breakpoints are defined only in terms of two points (all other information is
// calculated in constant time).
// These breakpoints are inserted directly into the set/tree, and their ordering remains
// invariant.
typedef std::pair<const Point*,const Point*> Breakpoint;

double computeIntersection(Breakpoint& b, double sweeplineY);


#endif /* VORONOI_H */

