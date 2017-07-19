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
} Breakpoint;

// VTreeNodes hold breakpoints as a pair of sites that "own" the intersecting parablas
// When traversing the tree, the intersection is (and must be) calculated on the fly,
// but in constant time. Through this, ordering remains invariant.
class VTreeNode {
    public:
        VTreeNode(Point* left, Point* right);
        ~VTreeNode();


        VTreeNode* parent;
        VTreeNode* left;
        VTreeNode* right;

        Color color;

        Breakpoint breakpoint;

        // In-order successor and predecessor
        // TODO adapt for rb tree
        VTreeNode* succ() const;
        VTreeNode* pred() const;

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
// Implementation based on windoro's gist (https://gist.github.com/windoro/1c8e458e9ab5e8e4c5bb)
class VTree {

    public:
        VTree();
        ~VTree();

        void destroyTree();
        void insert(double x); // Receieves site's x coord as argument

    private:
        void destroyTree(VTreeNode* leaf);
        void insert(double breakpoint, VTreeNode* node);

        static double computeBreakpoint(Point* p1, Point* p2);

        VTreeNode* root;
        VTreeNode* firstLeaf; // The leftmost leaf
};


#endif /* VORONOI_H */

