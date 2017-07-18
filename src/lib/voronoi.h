// Defines internal representation of Voronoi Diagram

#ifndef VORONOI_H
#define VORONOI_H

#include "point.h"
#include "circle.h"

enum EventType       { CircleE, PointE };
enum CompareResult   { GreaterThan, EqualTo, LessThan };
enum Color           { Red, Black };


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


typedef struct {
    Point* left;
    Point* right;

    int intersect;
} Breakpoint;

class VTreeNode {
    public:
        VTreeNode* parent;
        VTreeNode* left;
        VTreeNode* right;

        Color color;

        Breakpoint* bp;

        // In-order next and prev
        VTreeNode* succ();
        VTreeNode* pred();

    private:
        void rotateLeft();
        void rotateRight();

        VTreeNode* getMinimum(); // Returns minimal node in subtree
        VTreeNode* getMaximum(); // Returns maximal node in subtree
        VTreeNode* findSuccessorInAncestors();
        VTreeNode* findPredecessorInAncestors();
};


// Implemented as a RB tree (without modifications!)
// Neither parabolas nor arcs are explicitly represented in the tree.
// Rather, as null values in the RB tree, their identities can be derived from the
// breakpoints above.
// Implementation based on Thomas Niemann's original C source
class VTree {

    public:
        VTree();
        ~VTree();

        void destroyTree();
        void insert(double x); // Receieves site's x coord as argument

    private:
        void destroyTree(VTreeNode* leaf);
        void insert(double x);

        static double computeBreakpoint(Point* p1, Point* p2);

        VTreeNode* root;
        VTreeNode* firstLeaf; // The leftmost leaf
};


#endif /* VORONOI_H */

