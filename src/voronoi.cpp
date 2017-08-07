#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>
#include <stack>

double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->se->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}



BLNode::BLNode() { }

BLNode::BLNode(Breakpoint* b, DCEL_Edge* e) {
    this->p = nullptr;
    this->breakpoint = b;
    this->edge = e;
    this->lEvent = nullptr;
    this->rEvent = nullptr;
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
    this->lEvent = nullptr;
    this->rEvent = nullptr;
}

BLNode* BLNode::makeSentinel() {
    BLNode* sentinel = new BLNode;
    sentinel->lNode = sentinel->rNode = sentinel->parent = sentinel;
    sentinel->color = Black;
    sentinel->lEvent = sentinel->rEvent = nullptr;
    sentinel->edge = nullptr;
    return sentinel;
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

void BLNode::setLEvent(CircleEvent* lEvent) {
    this->lEvent = lEvent;
}

CircleEvent* BLNode::getLEvent() const {
    return this->lEvent;
}

void BLNode::setREvent(CircleEvent* rEvent) {
    this->rEvent = rEvent;
}

CircleEvent* BLNode::getREvent() const {
    return this->rEvent;
}

Point* BLNode::getPoint() const {
    return this->p;
}

double BLNode::computeIntersection(double sweeplineY) const {

    // As a hack to get site event insertion of points to work, and to allow insertion of
    // two breakpoints that appear to be at the same location, points are shited slightly
    Point* p;
    if((p = this->getPoint())) {
        return p->x + 0.00001;
    }

    if(std::get<0>(*this->getBreakpoint())->y == sweeplineY) {
        return std::get<0>(*this->getBreakpoint())->x;
    }
    if (std::get<1>(*this->getBreakpoint())->y == sweeplineY) {
        return std::get<1>(*this->getBreakpoint())->x;
    }

    double ay = std::get<0>(*this->getBreakpoint())->y - sweeplineY;
    double bx = std::get<1>(*this->getBreakpoint())->x - std::get<0>(*this->getBreakpoint())->x;
    double by = std::get<1>(*this->getBreakpoint())->y - sweeplineY;

    if(ay == by) {
        return (std::get<0>(*this->getBreakpoint())->x + std::get<1>(*this->getBreakpoint())->x) / 2;
    }

    double shiftedIntersect = (ay*bx + sqrt(ay*by*(pow(ay-by,2) + pow(bx,2))))/(ay-by);

    // TODO handle problem case of small denominators

    return shiftedIntersect + std::get<0>(*this->getBreakpoint())->x;
}



Beachline::Beachline(Event* e1, Event* e2) {
    this->root = this->insert(e1,e2);
    this->insert(e2,e1);
}

Beachline::~Beachline() {
    // TODO rewrite this without recursion using a stack
    destroyTree();
}

void Beachline::destroyTree(BLNode* node) {
    if(node != NULL) {
        destroyTree(node->lNode);
        destroyTree(node->rNode);
        delete node;
    }
}

void Beachline::destroyTree() {
    destroyTree(this->root);
    delete nil;
}

BLNode* Beachline::rotateLeft(BLNode* t) {
    BLNode* u = t->rNode;
    t->rNode = u->lNode;
    u->lNode = t;
    t->height = std::max(t->lNode->height,t->rNode->height) + 1;
    u->height = std::max(t->rNode->height,t->height) + 1;
    return u;
}

BLNode* Beachline::doubleRotateLeft(BLNode* t) {
    t->rNode = rotateRight(t->rNode);
    return rotateLeft(t);
}

BLNode* Beachline::rotateRight(BLNode* t) {
    BLNode* u = t->lNode;
    t->lNode = u->rNode;
    u->rNode = t;
    t->height = std::max(t->lNode->height,t->rNode->height) + 1;
    u->height = std::max(u->lNode->height,t->height) + 1;
    return u;
}

BLNode* Beachline::doubleRotateRight(BLNode* t) {
    t->lNode = rotateLeft(t->lNode);
    return rotateRight(t);
}

BLNode* Beachline::insert(BLNode* node, BLNode* t) {
    if(t == NULL) {
        t = node;
    } else if(node->computeIntersection(sweeplineY)
            < node->computeIntersection(sweeplineY)){
        t->lNode = insert(node,t->lNode);
        if(t->lNode->height - t->rNode->height == 2) {
            if(node->computeIntersection(sweeplineY)
             < t->computeIntersection(sweeplineY)) {
                t = rotateRight(t);
            } else {
                t = doubleRotateRight(t);
            }
        }
    } else {
        t->rNode = insert(node,t->rNode);
        if(t->rNode->height - t->lNode->height == 2) {
            if(node->computeIntersection(sweeplineY)
             > t->computeIntersection(sweeplineY)) {
                t = rotateLeft(t);
            } else {
                t = doubleRotateLeft(t);
            }
        }
    }

    t->height = std::max(t->lNode->height,t->rNode->height) + 1;
    return t;
}

BLNode* Beachline::getSuccessor(BLNode* x) const {
}

BLNode* Beachline::getPredecessor(BLNode* x) const {
}

BLNode* Beachline::insert(Event* e1, Event* e2) {
    Breakpoint* bp;
    BLNode* node;

    sweeplineY = e1->se->y > e2->se->y ? e1->se->y : e2->se->y;

    bp = new Breakpoint;
    *bp = std::make_pair(e1->se,e2->se);
    node = new BLNode(bp);

    DCEL_Face* face = new DCEL_Face;
    DCEL_Edge* e = new DCEL_Edge;

    face->edge = e;
    e->sibling = new DCEL_Edge;
    e->sibling->sibling = e;

    node->setEdge(e);
    this->insert(node,root); // root is null, so this node is made the root

    return node;
}

