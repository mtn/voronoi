// Defines internal representation of Voronoi Diagram

#pragma once

#include "point.h"
#include "circle.h"
#include "dcel.h"

#include <utility>
#include <queue>


enum EventType       { CircleE, SiteE };
enum NodeColor       { Red, Black };

extern double sweeplineY;


class BLNode;
typedef struct CircleEvent {
    CircleEvent(Circle* c, BLNode* b1, BLNode* b2);
    ~CircleEvent();

    bool deleted;

    Circle* c;

    BLNode* b1;
    BLNode* b2;
} CircleEvent;

typedef Point SiteEvent;

typedef struct Event {
    Event(CircleEvent* ce);
    Event(SiteEvent* se);

    EventType type;

    // One of these will be null
    // TODO use union
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
typedef std::priority_queue<Event*,std::vector<Event*>,CompareEvent> EventQueue;
extern EventQueue eq;


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
        int height;

        NodeColor color;

    private:
        // TODO use unions to enforce that nothing can exist simultaneously with a point

        Point* p;

        Breakpoint* breakpoint;
        DCEL_Edge* edge; // The voronoi edge the breakpoint is tracing out

        // The potential circle events when the left and right arcs will disappear are
        // stored. Thus, prev.right should agree with this.left, etc.
        // One or both could be null
        CircleEvent* lEvent;
        CircleEvent* rEvent;

};
typedef std::pair<BLNode*,BLNode*> NodePair;

/*
 * The beachline is implemented as a AVL tree that doesn't explicitly
 * store keys. Rather, they are computed dynamically each time there is
 * an insertion into the tree. Normally, having keys that change (as the
 * x positions of breakpoints do) would be problematic. Because ordering
 * is invariant in this problem, however, this is not a concern. Aside
 * from this, the only difference from a vanilla avl tree is that duplicates
 * are supported.
 */
class Beachline {
    public:
        Beachline(Event* e1, Event* e2);
        ~Beachline();

        void destroyTree();
        void remove(BLNode* node);

        bool isEmpty() const;
        BLNode* findMin() const;
        BLNode* getPredecessor(BLNode* node) const;
        BLNode* getSuccessor(BLNode* node) const;

        // Beachline operations
        void handleCircleEvent(CircleEvent* ce);
        void handleSiteEvent(SiteEvent* pe);

    private:
        BLNode* root;

        void destroyTree(BLNode* x);
        BLNode* remove(BLNode* node, BLNode* temp);

        NodePair* insert(Point* p);
        BLNode* insert(BLNode* node);
        BLNode* insert(Event* e1, Event* e2);
        BLNode* insert(BLNode* node, BLNode* t, BLNode* par);

        BLNode* rotateLeft(BLNode* node);
        BLNode* doubleRotateLeft(BLNode* node);
        BLNode* rotateRight(BLNode* node);
        BLNode* doubleRotateRight(BLNode* node);

        BLNode* findMin(BLNode* n) const;
        BLNode* findMax(BLNode* n) const;

        double height(BLNode* n);

        // Beachline operations
        void evaluateCircleEventCandidate(NodePair* n) const;
        void pushEvent(Circle* c, BLNode* l, BLNode* r) const;
};
