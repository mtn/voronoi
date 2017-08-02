// Defines internal representation of Voronoi Diagram

#pragma once

#include "point.h"
#include "circle.h"
#include "dcel.h"


#include <utility>
#include <set>
#include <vector>
#include <queue>

enum EventType       { CircleE, PointE };
enum CompareResult   { GreaterThan, EqualTo, LessThan };


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
    SiteEvent* pe;
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
        BLNode(Breakpoint* b, DCEL_Edge* e);

        void setBreakpoint(Breakpoint* bp);
        Breakpoint* getBreakpoint();

        void setEdge(DCEL_Edge* edge);
        DCEL_Edge* getEdge();

        void setLeft(CircleEvent* left);
        CircleEvent* getLeft();
        void setRight(CircleEvent* right);
        CircleEvent* getRight();

        Point* getPoint();

    private:
        // TODO use unions to enforce that nothing can exist simultaneously with a point

        Point* p;

        Breakpoint* breakpoint;
        DCEL_Edge* edge; // The voronoi edge the breakpoint is tracing out

        // The circle events when the left and right arcs will disappear are store
        // Thus, prev.right should agree with this.left, etc.
        CircleEvent* left; // Could be null
        CircleEvent* right;

};


double computeIntersection(const BLNode* b, double sweeplineY);
void handleCircleEvent(CircleEvent* ce);
void handleSiteEvent(SiteEvent* pe);

// BLNodes are compared by their breakpoints
class VState {
    public:
        static double sweeplineY; // this must be manually updated when appropriate

        struct CompareBLNode {
            bool operator()(const BLNode* b1, const BLNode* b2) const {
                return computeIntersection(b1,VState::sweeplineY)
                        <= computeIntersection(b2,VState::sweeplineY);
            }
        };

        std::set<BLNode,VState::CompareBLNode> beachline;
        std::priority_queue<Event*,std::vector<Event*>,CompareEvent> pq;

};



