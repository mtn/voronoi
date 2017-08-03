// Defines internal representation of Voronoi Diagram

#pragma once

#include "point.h"
#include "circle.h"
#include "dcel.h"


#include <utility>
#include <set>

#include <iostream>
using namespace std;

enum EventType       { CircleE, PointE };

extern double sweeplineY;


class BLNode;
typedef struct {
    bool deleted;

    Point* p1;
    Point* p2;
    Point* p3;
Circle* c;
    BLNode* node;
} CircleEvent;

typedef Point SiteEvent;

typedef struct {
    EventType type;

    // One of these will be null
    CircleEvent* ce;
    SiteEvent* se;
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
class BLNode {
    public:
        BLNode(Point* p); // Intended only for the initial insertion
        BLNode(Breakpoint* b);
        BLNode(Breakpoint* b, DCEL_Edge* e);

        void setBreakpoint(Breakpoint* bp);
        Breakpoint* getBreakpoint() const;

        void setEdge(DCEL_Edge* edge);
        DCEL_Edge* getEdge() const;

        void setLEvent(CircleEvent* left);
        CircleEvent* getLEvent() const;
        void setREvent(CircleEvent* right);
        CircleEvent* getREvent() const;

        Point* getPoint() const;

        double computeIntersection(double sweeplineY) const;

        // Left and right nodes in the RB tree
        BLNode* lNode;
        BLNode* rNode;

    private:
        // TODO use unions to enforce that nothing can exist simultaneously with a point

        Point* p;

        Breakpoint* breakpoint;
        DCEL_Edge* edge; // The voronoi edge the breakpoint is tracing out

        // The circle events when the left and right arcs will disappear are store
        // Thus, prev.right should agree with this.left, etc.
        CircleEvent* lEvent; // Could be null
        CircleEvent* rEvent;

};

/* Defines a modified RB tree that supports dynamic keys (which is fine here * because the order of parabolas along the beachline being encoded is
 * invariant). */

// BLNodes are compared by their breakpoints
struct CompareBLNode {
    bool operator()(const BLNode* b1, const BLNode* b2) const {
        std::cout << "comarison invoked with sweepline = " << sweeplineY << std::endl;
        cout << b1->getBreakpoint()->first->x << "," << b1->getBreakpoint()->first->y << ": " << b1->getBreakpoint()->second->x << "," << b1->getBreakpoint()->second->y << ": " << b1->computeIntersection(sweeplineY) << endl;
        cout << b2->getBreakpoint()->first->x << "," << b2->getBreakpoint()->first->y << ": " << b2->getBreakpoint()->second->x << "," << b2->getBreakpoint()->second->y << ": " << b2->computeIntersection(sweeplineY) << endl;
        cout << endl;
        return b1->computeIntersection(sweeplineY)
            <= b2->computeIntersection(sweeplineY);
    }
};

class Beachline {
    public:
        Beachline();
        Beachline(BLNode* root);
        ~Beachline();

        void insert(Point* p);
        void insert(Breakpoint* bp);
        void destroyTree();

        void insertBreakpoint(Event* e1, Event* e2); // Only used for the first insertion
        void insertPoint(Event* e);
        // TODO handlesite should check and manage the first case instead of main

        void handleCircleEvent(CircleEvent* ce);
        void handleSiteEvent(SiteEvent* pe);

    private:
        void destroyTree(BLNode* node);
        void insert(Point* p, BLNode* node);
        void insert(Breakpoint* bp, BLNode* node);

        BLNode* root;
};


