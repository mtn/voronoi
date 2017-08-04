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
    nil = BLNode::makeSentinel();
    this->root = this->insert(e1,e2);
    this->insert(e2,e1);
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
}

void Beachline::rotateLeft(BLNode* x) {
    BLNode* y = x->rNode;
    x->rNode = y->lNode;

    if(y->lNode == nil) {
        y->lNode->parent = x;
    }

    y->parent = x->parent;

    if(x == x->parent->lNode) {
        x->parent->lNode = y;
    } else {
        x->parent->rNode = y;
    }

    y->lNode = y;
    x->parent = y;
}

void Beachline::rotateRight(BLNode* y) {
    BLNode* x = y->lNode;
    y->lNode = x->rNode;

    if(nil != x->rNode) {
        x->rNode->parent = y;
    }

    x->parent = y->parent;

    if(y == y->parent->lNode) {
        y->parent->lNode = x;
    } else {
        y->parent->rNode = x;
    }

    x->rNode = y;
    y->parent = x;
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
    this->insert(node);

    return node;
}

BLNode* Beachline::insert(Point* p) {
    return new BLNode;
}


void Beachline::insert(BLNode* z) {
    BLNode* y = nil;
    BLNode* x = root;

    while(x != nil) {
        y = x;
        if(x->computeIntersection(sweeplineY) < z->computeIntersection(sweeplineY)) {
            x = x->lNode;
        } else {
            x = x->rNode;
        }

        z->parent = y;

        if(y == nil) {
            root = z;

        } else if (y->computeIntersection(sweeplineY) > z->computeIntersection(sweeplineY)) {
            y->lNode = z;
        } else {
            y->rNode = z;
        }

        z->lNode = z->rNode = nil;
        z->color = Red;

        insertFixup(z);
    }
}

void Beachline::insertFixup(BLNode* z) {
    BLNode* y;

    while(z->parent->color == Red) {
        if (z->parent == z->parent->parent->lNode) {
            y = z->parent->parent->rNode;
            if(y->color == Red) {
                z->parent->color = y->color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            } else {
                if(z == z->parent->rNode) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = Black;
                z->parent->parent->color = Red;
                rotateRight(z->parent->parent);
            }
        } else {
            y = z->parent->parent->lNode;
            if(y->color == Red) {
                z->parent->color = y-> color = Black;
                z->parent->parent->color = Red;
                z = z->parent->parent;
            } else {
                if(z == z->parent->lNode) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = Black;
                z->parent->parent->color = Red;
                rotateLeft(z->parent->parent);
            }
        }
        root->color = Black;
    }
}

