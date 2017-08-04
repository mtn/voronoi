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
enum NodeColor       { Red, Black };

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
    friend class Beachline;

    public:
        BLNode();
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
        BLNode* parent;

        NodeColor color;

    protected:
        static BLNode* makeSentinel();

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

// BLNodes are compared by the x coordinate of their breakpoints
struct CompareBLNode {
    bool operator()(const BLNode* b1, const BLNode* b2) const {
        return b1->computeIntersection(sweeplineY)
            <= b2->computeIntersection(sweeplineY);
    }
};

/*
 * The beachline is implemented as a RB tree that doesn't explicitly
 * store keys. Rather, they are computed dynamically each time there is
 * an insertion into the tree. Normally, having keys that change (as the
 * x positions of breakpoints do) would be problematic. Because ordering
 * is invariant in this problem, however, this is not a concern.
 */
// TODO handlesite should check and manage the first case instead of main
class Beachline {
    public:
        Beachline(Event* e1, Event* e2);
        ~Beachline();

        void destroyTree();

        BLNode* insert(Point* p);

        void deleteNode(BLNode* node);

        BLNode* getPredecessor(BLNode* node);
        BLNode* getSuccessor(BLNode* node);

        void handleCircleEvent(CircleEvent* ce);
        void handleSiteEvent(SiteEvent* pe);

    protected:
        void rotateLeft(BLNode* x);
        void rotateRight(BLNode* y);

        BLNode* root;
        BLNode* nil;


    private:
        void insert(BLNode* node);
        void insertHelper(BLNode* node);

        void destroyTree(BLNode* node);

        BLNode* insert(Point* p, BLNode* node);
        BLNode* insert(Breakpoint* bp, BLNode* node);
        BLNode* insert(Event* e1, Event* e2);

};


