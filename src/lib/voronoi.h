// Defines internal representation of Voronoi Diagram

#ifndef VORONOI_H
#define VORONOI_H

#include "point.h"
#include "circle.h"
/* class Point; */
/* class Circle; */

enum EventType { CircleE, PointE };

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

double getEventPriority(const Event* e);
struct CompareEvent {
    bool operator()(const Event* e1, const Event* e2) const {
        double e1Y = getEventPriority(e1);
        double e2Y = getEventPriority(e2);
        return e1Y > e2Y;
    }
};


class Node {
    public:
        Node* parent;
        Node* left;
        Node* right;

        double breakpoint;
        Point* site;

        // In-order next and prev
        Node* succ();
        Node* pred();

    private:
        Node* getMinimum(); // Returns minimal node in subtree
        Node* getMaximum(); // Returns maximal node in subtree
        Node* findSuccessorInAncestors();
        Node* findPredecessorInAncestors();
};

// Implementation based on http://www.cprogramming.com/tutorial/lesson18.html
class VTree {
    public:
        VTree();
        ~VTree();

        void destroyTree();
        void insert(double x); // Receieves site's x coord as argument

    private:
        void destroyTree(Node* leaf);
        void insert(double x, Node* leaf);

        static double computeBreakpoint(Point* p1, Point* p2);

        Node* root;
        Node* firstLeaf;
};


#endif /* VORONOI_H */

