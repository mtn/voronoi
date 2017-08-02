#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>


double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->se->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


double computeIntersection(const BLNode* b, double sweeplineY) {

    // As a hack to get site event insertion of points to work, and to allow insertion of
    // two breakpoints that appear to be at the same location, points are shited slightly
    Point* p;
    if((p = b->getPoint())) {
        return p->x + 0.00001;
    }

    if(std::get<0>(*b->getBreakpoint())->y == sweeplineY) {
        return std::get<0>(*b->getBreakpoint())->x;
    }
    if (std::get<1>(*b->getBreakpoint())->y == sweeplineY) {
        return std::get<1>(*b->getBreakpoint())->x;
    }

    double ay = std::get<0>(*b->getBreakpoint())->y - sweeplineY;
    double bx = std::get<1>(*b->getBreakpoint())->x - std::get<0>(*b->getBreakpoint())->x;
    double by = std::get<1>(*b->getBreakpoint())->y - sweeplineY;

    if(ay == by) {
        return (std::get<0>(*b->getBreakpoint())->x + std::get<1>(*b->getBreakpoint())->x) / 2;
    }

    double shiftedIntersect = (ay*bx + sqrt(ay*by*(pow(ay-by,2) + pow(bx,2))))/(ay-by);

    // TODO handle problem case of small denominators

    return shiftedIntersect + std::get<0>(*b->getBreakpoint())->x;
}


void handleSiteEvent(SiteEvent* se) {


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

BLNode::BLNode(Breakpoint* b) {
    this->p = nullptr;
    this->breakpoint = b;
    this->edge = new DCEL_Edge;
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
Breakpoint* BLNode::getBreakpoint() const {
    return this->breakpoint;
}

void BLNode::setEdge(DCEL_Edge* edge) {
    this->edge = edge;
}
DCEL_Edge* BLNode::getEdge() const {
    return this->edge;
}

void BLNode::setLeft(CircleEvent* left) {
    this->left = left;
}
CircleEvent* BLNode::getLeft() const {
    return this->left;
}

void BLNode::setRight(CircleEvent* right) {
    this->right = right;
}
CircleEvent* BLNode::getRight() const {
    return this->right;
}

Point* BLNode::getPoint() const {
    return this->p;
}


void Beachline::insertBreakpoint(Event* e1, Event* e2) {
    Breakpoint* bp;
    BLNode* node;

    bp = new Breakpoint;
    *bp = std::make_pair(e1->se,e2->se);
    node = new BLNode(bp);

    DCEL_Face* face = new DCEL_Face;
    DCEL_Edge* e = new DCEL_Edge;

    face->edge = e;
    e->sibling = new DCEL_Edge;
    e->sibling->sibling = e;

    node->setEdge(e);
    this->set.insert(node);
}

void Beachline::insertPoint(Event* e) {

}

