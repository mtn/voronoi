#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>
#include <stack>

#include <iostream>
using namespace std;

double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->se->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


CircleEvent::CircleEvent(Circle* c, BLNode* b1, BLNode* b2) {
    deleted = false;
    this->c = c;
    this->b1 = b1;
    this->b2 = b2;
}

CircleEvent::~CircleEvent() {
    delete c;
}

BLNode::BLNode() {
    p = nullptr;
    breakpoint = nullptr;
    edge = nullptr;
    lEvent = rEvent = nullptr;
    lNode = rNode = parent = nullptr;
}

BLNode::BLNode(Breakpoint* b, DCEL_Edge* e) {
    p = nullptr;
    breakpoint = b;
    edge = e;
    lEvent = rEvent = nullptr;
    lNode = rNode = parent = nullptr;
}

BLNode::BLNode(Breakpoint* b) {
    p = nullptr;
    breakpoint = b;
    edge = new DCEL_Edge;
    lNode = rNode = parent = nullptr;
}

// Used only in the first case (add a single leaf)
BLNode::BLNode(Point* p) {
    this->p = p;
    breakpoint = nullptr;
    edge = nullptr;
    lEvent = nullptr;
    rEvent = nullptr;
    lNode = rNode = parent = nullptr;
}

void BLNode::setBreakpoint(Breakpoint* bp) {
    p = nullptr;
    breakpoint = bp;
}
Breakpoint* BLNode::getBreakpoint() const {
    return breakpoint;
}

void BLNode::setEdge(DCEL_Edge* edge) {
    this->edge = edge;
}

DCEL_Edge* BLNode::getEdge() const {
    return edge;
}

void BLNode::setLEvent(CircleEvent* lEvent) {
    this->lEvent = lEvent;
}

CircleEvent* BLNode::getLEvent() const {
    return lEvent;
}

void BLNode::setREvent(CircleEvent* rEvent) {
    this->rEvent = rEvent;
}

CircleEvent* BLNode::getREvent() const {
    return rEvent;
}

Point* BLNode::getPoint() const {
    return p;
}

double BLNode::computeIntersection(double sweeplineY) const {

    // As a hack to get site event insertion of points to work, and to allow insertion of
    // two breakpoints that appear to be at the same location, points are shited slightly
    Point* p;
    if((p = getPoint())) {
        return p->x;
    }

    if(getBreakpoint()->first->y == sweeplineY) {
        return getBreakpoint()->first->x;
    }
    if(getBreakpoint()->second->y == sweeplineY) {
        return getBreakpoint()->second->x;
    }


    double ay = getBreakpoint()->first->y - sweeplineY;
    double bx = getBreakpoint()->second->x - getBreakpoint()->first->x;
    double by = getBreakpoint()->second->y - sweeplineY;

    if(ay == by) {
        return (getBreakpoint()->first->x + getBreakpoint()->second->x) / 2;
    }

    double shiftedIntersect = (ay*bx + sqrt(ay*by*(pow(ay-by,2) + pow(bx,2))))/(ay-by);

    // TODO handle problem case of small denominators

    return shiftedIntersect + getBreakpoint()->first->x;
}



Beachline::Beachline(Event* e1, Event* e2) {
    root = nullptr; // Required to prevent root from getting some address on the stack
    root = insert(e1,e2);
    insert(e2,e1);
}

Beachline::~Beachline() {
    // TODO rewrite without recursion using a stack
    destroyTree(root);
}

void Beachline::destroyTree(BLNode* node) {
    if(node != nullptr) {
        destroyTree(node->lNode);
        destroyTree(node->rNode);
        delete node;
    }
}

BLNode* Beachline::rotateLeft(BLNode* t) {
    BLNode* u = t->rNode;
    t->rNode = u->lNode;

    if(t->rNode) {
        t->rNode->parent = t;
    }

    u->lNode = t;
    u->parent = t->parent;
    t->parent = u;

    t->height = std::max(height(t->lNode),height(t->rNode)) + 1;
    u->height = std::max(height(t->rNode),height(t)) + 1;

    return u;
}

BLNode* Beachline::doubleRotateLeft(BLNode* t) {
    t->rNode = rotateRight(t->rNode);
    return rotateLeft(t);
}

BLNode* Beachline::rotateRight(BLNode* t) {
    BLNode* u = t->lNode;
    t->lNode = u->rNode;

    if(t->lNode) {
        t->lNode->parent = t;
    }

    u->rNode = t;
    u->parent = t->parent;
    t->parent = u;

    t->height = std::max(height(t->lNode),height(t->rNode)) + 1;
    u->height = std::max(height(u->lNode),height(t)) + 1;

    return u;
}

BLNode* Beachline::doubleRotateRight(BLNode* t) {
    t->lNode = rotateLeft(t->lNode);
    return rotateRight(t);
}

BLNode* Beachline::insert(BLNode* node, BLNode* t, BLNode* par) {
    if(t == nullptr) {
        t = node;
        t->height = 0;
        t->lNode = t->rNode = nullptr;
        t->parent = par;
    } else if(node->computeIntersection(sweeplineY)
            < t->computeIntersection(sweeplineY)) {

        t->lNode = insert(node,t->lNode,t);

        if(height(t->lNode) - height(t->rNode) == 2) {

            if(node->computeIntersection(sweeplineY)
             < t->lNode->computeIntersection(sweeplineY)) {
                t = rotateRight(t);
            } else {
                t = doubleRotateRight(t);
            }

        }

    } else {

        t->rNode = insert(node,t->rNode,t);

        if(height(t->rNode) - height(t->lNode) == 2) {

            if(node->computeIntersection(sweeplineY)
             >= t->rNode->computeIntersection(sweeplineY)) {
                t = rotateLeft(t);
            } else {
                t = doubleRotateLeft(t);
            }

        }

    }

    t->height = std::max(height(t->lNode),height(t->rNode)) + 1;

    return t;
}

BLNode* Beachline::insert(BLNode* node) {
    return insert(node,root,nullptr);
}

BLNode* Beachline::insert(Event* e1, Event* e2) {
    Breakpoint* bp;
    BLNode* node;

    sweeplineY = e1->se->y > e2->se->y ? e1->se->y : e2->se->y;

    bp = new Breakpoint;
    *bp = std::make_pair(e1->se,e2->se);
    node = new BLNode(bp);
    node->lNode = node->rNode = node->parent = nullptr;

    DCEL_Face* face = new DCEL_Face;
    DCEL_Edge* e = new DCEL_Edge;

    face->edge = e;
    e->sibling = new DCEL_Edge;
    e->sibling->sibling = e;

    node->setEdge(e);
    root = insert(node);

    return node;
}

void Beachline::insert(Point* p) {
    Breakpoint* bp;
    BLNode *pred, *succ;
    BLNode *n1, *n2;

    n1 = new BLNode(p);
    n2 = nullptr;
    root = insert(n1);

    pred = getPredecessor(n1);
    if(pred) {
        bp = new Breakpoint;
        *bp = make_pair(pred->getBreakpoint()->second,p);
        n1->setBreakpoint(bp);
    }

    succ = getSuccessor(n1);
    if(succ) {
        bp = new Breakpoint;
        *bp = make_pair(p,succ->getBreakpoint()->first);

        if(n1->getBreakpoint() != nullptr) {
            n2 = new BLNode;
            n2->setBreakpoint(bp);
        } else {
            n1->setBreakpoint(bp);
        }
    }

    if(n1->getBreakpoint() == nullptr) {
        remove(n1);
    }
    if(n2 != nullptr) {
        insert(n2);
    }

}

BLNode* Beachline::findMin() const {
    return findMin(root);
}

BLNode* Beachline::findMin(BLNode* n) const { if(n == nullptr) {
        return nullptr;
    } else if(n->lNode == nullptr){
        return n;
    } else {
        return findMin(n->lNode);
    }
}

BLNode* Beachline::findMax(BLNode* n) const {
    if(n == nullptr) {
        return nullptr;
    } else if(n->rNode == nullptr) {
        return n;
    } else {
        return findMax(n->rNode);
    }
}

BLNode* Beachline::getSuccessor(BLNode* n) const {
    if(n->rNode != nullptr) {
        return findMin(n->rNode);
    }

    BLNode* p = n->parent;
    while(p != nullptr && n == p->rNode) {
        n = p;
        p = p->parent;
    }

    return p;
}

BLNode* Beachline::getPredecessor(BLNode* n) const {
    if(n->lNode != nullptr) {
        return findMax(n->lNode);
    }

    BLNode* p = n->parent;
    while(p != nullptr && n == p->lNode) {
        n = p;
        p = p->parent;
    }

    return p;
}

void Beachline::remove(BLNode* n) {
    root = remove(n,root);
}

// Fails if the node requested for removal is not in tree
BLNode* Beachline::remove(BLNode* n, BLNode* t) {
    BLNode* temp;

    if(t == nullptr) {
        return nullptr;
    }

    // Searching
    else if(n->computeIntersection(sweeplineY)
            < t->computeIntersection(sweeplineY)) {
        t->lNode = remove(n,t->lNode);
    } else if(n->computeIntersection(sweeplineY)
            >= t->computeIntersection(sweeplineY)) {
        t->rNode = remove(n,t->rNode);
    }

    // Element found with 2 children
    // Finds least node in right subtree and swaps it in
    else if(t->lNode && t->rNode) {
        temp = t;
        t = findMin(t->rNode);
        t->parent = temp->parent;
        t->lNode = temp->lNode;
        t->rNode = temp->rNode;
        delete temp;
    }

    // Element found with <2 children
    // If there's a child, it's shifted up. Otherwise, we're at a leaf and removal
    // is trivial.
    else {
        temp = t;
        if(t->lNode == nullptr) {
            t->rNode->parent = t->parent;
            t = t->rNode;
        } else if(t->rNode == nullptr) {
            t->lNode->parent = t->parent;
            t = t->lNode;
        }
        delete temp;
    }
    if(t == nullptr) {
        return t;
    }

    t->height = std::max(height(t->lNode),height(t->rNode));

    // Balancing Checks

    if(height(t->lNode) - height(t->rNode) == 2) {

        if(height(t->lNode->lNode) - height(t->lNode->rNode) == 1) {
            return rotateLeft(t);
        } else {
            return doubleRotateLeft(t);
        }

    } else if(height(t->rNode) - height(t->lNode) == 2) {

        if(height(t->rNode->rNode) - height(t->rNode->lNode) == 1) {
            return rotateRight(t);
        } else{
            return doubleRotateRight(t);
        }

    }

    return t;
}

double Beachline::height(BLNode* n) {
    return n == NULL ? -1 : n->height;
}

void Beachline::handleSiteEvent(SiteEvent* se) {
    sweeplineY = se->y;
    insert(se);
    // add 2 half edges
    // check for circle events. add any that are found to pq
}

void Beachline::handleCircleEvent(CircleEvent* ce) {
    sweeplineY = ce->c->center->y + ce->c->radius;
    // add vertex to appropriate edge into dcel
    // delete
}

// Enforces that b2 should be the successor of b1
CircleEvent* Beachline::isCircleEventCandidate(BLNode* b1, BLNode* b2) const {

    if(getSuccessor(b1) != b2) {
        return nullptr;
    }

    const Point* x = b1->getBreakpoint()->first;
    const Point* y = b1->getBreakpoint()->second;
    const Point* z = b2->getBreakpoint()->first;
    Circle* c = Circle::computeCircumcircle(x,y,z);

    if(c->center->y + c->radius < sweeplineY) {
        return nullptr;
    }

    CircleEvent* ce = new CircleEvent(c,b1,b2);
    b1->rEvent = ce;
    b2->lEvent = ce;

    return ce;
}

