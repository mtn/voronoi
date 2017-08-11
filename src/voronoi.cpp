#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <cmath>
#include <stack>

#include <iostream>
using namespace std;

double getEventPriority(const Event* e) {
    if(e->type == SiteE) {
        return e->se->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}


Event::Event(CircleEvent* ce) {
    type = CircleE;
    this->ce = ce;
    se = nullptr;
}

Event::Event(SiteEvent* se) {
    type = SiteE;
    this->se = se;
    ce = nullptr;
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

BLNode::~BLNode() {
    cout << "Node " << this << " is being deleted." << endl;
    if(breakpoint) {
        delete breakpoint;
        breakpoint = nullptr;
    }
}

BLNode::BLNode(Breakpoint* b, DCEL_Edge* e) : breakpoint(b), edge(e) {
    p = nullptr;
    lEvent = rEvent = nullptr;
    lNode = rNode = parent = nullptr;
}

BLNode::BLNode(Breakpoint* b) : breakpoint(b) {
    p = nullptr;
    edge = new DCEL_Edge;
    lNode = rNode = parent = nullptr;
}

// Used only in the first case (add a single leaf)
BLNode::BLNode(Point* p) : p(p) {
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
    cout << "Being used" << endl;
    destroyTree(root);
    root = nullptr;
}

bool Beachline::isEmpty() const {
    return (bool)root;
}

void Beachline::destroyTree(BLNode* node) {
    if(node != nullptr) {
        destroyTree(node->lNode);
        destroyTree(node->rNode);
        delete node;
        node = nullptr;
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

    updateHeight(t);
    updateHeight(u);

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

    updateHeight(t);
    updateHeight(u);

    return u;
}

BLNode* Beachline::doubleRotateRight(BLNode* t) {
    t->lNode = rotateLeft(t->lNode);
    return rotateRight(t);
}

BLNode* Beachline::insert(BLNode* node, BLNode* t, BLNode* par) {
    if(t == nullptr) {
        /* cout << "Inserted a node" << endl; */
        t = node;
        /* cout << "Node: " << node << endl; */
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

    updateHeight(t);

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

NodePair* Beachline::insert(Point* p) {
    Breakpoint* bp;
    BLNode *pred, *succ;
    BLNode *n1, *n2;

    NodePair* nodes = new NodePair;
    n1 = new BLNode(p);
    n2 = new BLNode;
    root = insert(n1);

    pred = getPredecessor(n1);
    succ = getSuccessor(n1);
    if(pred) {
        bp = new Breakpoint;
        *bp = make_pair(pred->getBreakpoint()->second,p);
        n1->setBreakpoint(bp);

        bp = new Breakpoint;
        *bp = make_pair(p,pred->getBreakpoint()->second);
        n2->setBreakpoint(bp);

    } else if(succ){
        bp = new Breakpoint;
        *bp = make_pair(p,succ->getBreakpoint()->first);
        n1->setBreakpoint(bp);

        bp = new Breakpoint;
        *bp = make_pair(succ->getBreakpoint()->first,p);
        n2->setBreakpoint(bp);
    }
    root = insert(n2);

    *nodes = std::make_pair(n1,n2);
    return nodes;
}

BLNode* Beachline::findMin() const {
    cout << "getting min" << endl;
    BLNode* min = findMin(root);
    cout << "got min" << endl;
    return min;
    /* return findMin(root); */
}

BLNode* Beachline::findMin(BLNode* n) const {
    if(n == nullptr) {
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
    cout << "starting removal" << endl;
    root = remove(n,root);
}

// Fails if the node requested for removal is not in tree
BLNode* Beachline::remove(BLNode* n, BLNode* t) {
    double nIntersect, tIntersect;
    BLNode* temp;

    // Search returned null
    if(t == nullptr) {
        return nullptr;
    } else {
        nIntersect = n->computeIntersection(sweeplineY);
        tIntersect = t->computeIntersection(sweeplineY);
    }

    // Searching
    if(nIntersect < tIntersect) {
        remove(n,t->lNode);
    } else if(nIntersect > tIntersect) {
        remove(n,t->rNode);
    }

    // Element found with 2 children
    // Swaps the successor into its place
    // Shouldn't ever be used, but included for completeness
    else if(t->lNode && t->rNode) {
        cout << "being used" << endl;
        temp = t;
        t = getSuccessor(t);

        t->parent = temp->parent;
        t->lNode = temp->lNode;
        t->rNode = temp->rNode;

        cout << "shouldn't be here" << endl;
        delete temp;
    }

    // Element found with <2 children
    // If there's a child, it's shifted up. Otherwise, we're at a leaf and removal
    // is trivial.
    else {
        cout << "GOOD element found with less than 2 children" << endl;
        if(t->lNode == nullptr && t->rNode == nullptr) {
            cout << "on branch 1" << endl;
            removeAndUpdateParent(t,nullptr);
            cout << "past branch 1" << endl;
        } else if(t->rNode != nullptr) {
            cout << "on branch 2" << endl;
            t->rNode->parent = t->parent;
            removeAndUpdateParent(t,t->rNode);
            cout << "past branch 2" << endl;
        } else {
            cout << "on branch 3" << endl;
            t->lNode->parent = t->parent;
            removeAndUpdateParent(t,t->lNode);
            cout << "past branch 3" << endl;
        }

    }

    // Search leads to null
    if(t == nullptr) {
        return nullptr;
    }

    updateHeight(t);

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

void Beachline::removeAndUpdateParent(BLNode* t, BLNode* updateVal) {
    if(t->parent) {
        if(t->parent->lNode == t) {
            t->parent->lNode = updateVal;
        } else {
            t->parent->rNode = updateVal;
        }

        updateHeight(t->parent);
    }

    cout << "t: " << t << endl;
    delete t;
    t = nullptr;
}

double Beachline::height(BLNode* n) {
    return n == nullptr ? -1 : n->height;
}

void Beachline::updateHeight(BLNode* n) {
    if(n) {
        n->height = std::max(height(n->lNode),height(n->rNode)) + 1;
    }
}

void Beachline::handleSiteEvent(SiteEvent* se) {
    sweeplineY = se->y;
    NodePair* nodes = insert(se);

    BLNode* pred = getPredecessor(nodes->first);
    if(pred && pred->rEvent) {
        pred->rEvent->deleted = true;
        /* cout << "Deleted an old ce" << endl; */
    }
    BLNode* succ = getSuccessor(nodes->second);
    if(succ && succ->lEvent) {
        succ->lEvent->deleted = true;
        /* cout << "Deleted an old ce" << endl; */
    }

    evaluateCircleEventCandidate(nodes);

    // add 2 half edges to the dcel

    /* delete nodes; */
}

void Beachline::handleCircleEvent(CircleEvent* ce) {
    sweeplineY = ce->c->center->y + ce->c->radius;

    if(ce->deleted) {
        return;
    }

    // add vertex, set it's loc as the center of the circle
    // delete
}

// Enforces that b2 should be the successor of b1
void Beachline::evaluateCircleEventCandidate(NodePair* n) const {

    if(getSuccessor(n->first) != n->second) {
        return;
    }

    BLNode* pred = getPredecessor(n->first);
    if(pred) {
        const Point* q = pred->getBreakpoint()->first;
        const Point* r = n->first->getBreakpoint()->first;
        const Point* s = n->first->getBreakpoint()->second;
        Circle* c = Circle::computeCircumcircle(q,r,s);
        pushEvent(c,n->first,pred);
    }

    BLNode* succ = getSuccessor(n->second);
    if(succ) {
        const Point* q = succ->getBreakpoint()->second;
        const Point* r = n->second->getBreakpoint()->first;
        const Point* s = n->second->getBreakpoint()->second;
        Circle* c = Circle::computeCircumcircle(q,r,s);
        pushEvent(c,succ,n->second);
    }
}

// l is the node that will take the event as it's lEvent, so the naming
// is actually opposite ordering along the beachline
void Beachline::pushEvent(Circle* c, BLNode* l, BLNode* r) const {
    if(c->center->y + c->radius > sweeplineY) {
        CircleEvent* ce = new CircleEvent(c,l,r);
        l->lEvent = ce;
        r->rEvent = ce;
        eq.push(new Event(ce));
        /* cout << "Pushed CE onto queue" << endl; */
    }
}

