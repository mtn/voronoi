#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>


double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->pe->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


double computeIntersection(Breakpoint& b, double sweeplineY) {
    if(std::get<0>(b)->y == sweeplineY) {
        return std::get<0>(b)->x;
    }
    if (std::get<1>(b)->y == sweeplineY) {
        return std::get<1>(b)->x;
    }

    double ay = std::get<0>(b)->y - sweeplineY;
    double bx = std::get<1>(b)->x - std::get<0>(b)->x;
    double by = std::get<1>(b)->y - sweeplineY;

    if(ay == by) { return (std::get<0>(b)->x + std::get<1>(b)->x) / 2;
    }

    double shiftedIntersect = (ay*bx + sqrt(ay*by*(pow(ay-by,2) + pow(bx,2))))/(ay-by);

    // TODO handle problem case of small denominators
    return shiftedIntersect + std::get<0>(b)->x;
}


void handleSiteEvent(SiteEvent* pe) {

}

void handleCircleEvent(CircleEvent* ce) {

}


BLNode::BLNode(Breakpoint* b, DCEL_Edge* e) {
    this->p = nullptr;
    this->breakpoint = b;
    this->edge = e;
    this->left = nullptr;
    this->right = nullptr;
}

// Used only in the first case (add a single leaf)
BLNode::BLNode(Point* p) {
    this->p = p;
    this->breakpoint = nullptr;
    this->edge = nullptr;
    this->left = nullptr;
    this->right = nullptr;
}

void BLNode::setBreakpoint(Breakpoint* bp) {
    this->p = nullptr;
    this->breakpoint = bp;
}
Breakpoint* BLNode::getBreakpoint() {
    return this->breakpoint;
}

void BLNode::setEdge(DCEL_Edge* edge) {
    this->edge = edge;
}
DCEL_Edge* BLNode::getEdge() {
    return this->edge;
}

void BLNode::setLeft(CircleEvent* left) {
    this->left = left;
}
CircleEvent* BLNode::getLeft() {
    return this->left;
}

void BLNode::setRight(CircleEvent* right) {
    this->right = right;
}
CircleEvent* BLNode::getRight() {
    return this->right;
}

Point* BLNode::getPoint() {
    return this->p;
}
