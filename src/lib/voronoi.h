// Defines internal representation of Voronoi Diagram

#pragma once

#include "point.h"
#include "circle.h"
#include "dcel.h"

#include <utility>

enum EventType       { CircleE, PointE };
enum CompareResult   { GreaterThan, EqualTo, LessThan };


struct TNode;
typedef struct {
    Point* p1;
    Point* p2;
    Point* p3;

    Circle* c;
    struct TNode* node;
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


typedef std::pair<const Point*,const Point*> Breakpoint;
typedef struct TNode {
    bool deletedLeft; // Probably temporary; will test alternate method later
    bool deletedRight;

    Breakpoint* breakpoint;
    DCEL_Edge* edge; // The voronoi edge the breakpoint is tracing out

    // The circle events when the left and right arcs will disappear are store
    // Thus, prev.right should agree with this.left, etc.
    CircleEvent* left; // Could be nil
    CircleEvent* right;

    TNode(); // Intended only for the initial insertion
    TNode(Breakpoint* b, DCEL_Edge* e);
} TNode;

// TNodes are compared by their breakpoints
struct CompareTNode {
    bool operator()(const TNode* t1, const TNode* t2) const {
        return t1->breakpoint != t2->breakpoint;
    }
};

double computeIntersection(Breakpoint& b, double sweeplineY);
void handleCircleEvent(CircleEvent* ce);
void handleSiteEvent(PointEvent* pe);

