#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <limits>


double getEventPriority(const Event* e) {
    if(e->type == PointE) {
        return e->pe->y;
    } else {
        return e->ce->c->center->y + e->ce->c->radius;
    }
}

VTreeNode* VTreeNode::succ() {
    if(this->right != NULL) {
        return this->right->getMinimum();
    } else {
        return this->findSuccessorInAncestors();
    }
}

VTreeNode* VTreeNode::getMinimum() {
    if(this->left != NULL) {
        return this->left->getMinimum();
    } else {
        return this;
    }
}

VTreeNode* VTreeNode::findSuccessorInAncestors() {
    if(this->parent && this == this->parent->right) {
        return this->parent->findSuccessorInAncestors();
    } else {
        return this->parent;
    }
}

VTreeNode* VTreeNode::pred() {
    if(this->left != NULL) {
        return this->left->getMaximum();
    } else {
        return this->findPredecessorInAncestors();
    }
}

VTreeNode* VTreeNode::getMaximum() {
    if(this->right != NULL) {
        return this->right->getMaximum();
    } else {
        return this;
    }
}

VTreeNode* VTreeNode::findPredecessorInAncestors() {
    if(this->parent && this == this->parent->left) {
        return this->parent->findPredecessorInAncestors();
    } else {
        return this->parent;
    }
}


VTree::VTree() {
    root = NULL;
}

VTree::~VTree() {
    destroyTree();
}

void VTree::destroyTree() {
    destroyTree(root);
}

void VTree::destroyTree(VTreeNode* leaf) {
    if(leaf != NULL) {
        destroyTree(leaf->left);
        destroyTree(leaf->right);
        delete leaf;
    }
}

void VTree::insert(double x) {
    if(root != NULL) {
        insert(x,root);
    } else {
        root = new VTreeNode;
        root->breakpoint = x;
        root->left = NULL;
        root->right = NULL;
    }
}

void VTree::insert(double breakpoint, VTreeNode* node) {
    if(breakpoint < node->breakpoint) {
        if(node->left != NULL) {
            insert(breakpoint,node->left);
        } else {
            node->left = new VTreeNode;
            node->left->breakpoint = breakpoint;
            node->left->left = NULL;
            node->left->right= NULL;
        }
    } else {
        if(node->right != NULL) {
            insert(breakpoint,node->right);
        } else {
            node->right = new VTreeNode;
            node->right->breakpoint = breakpoint;
            node->right->left = NULL;
            node->right->right= NULL;
        }
    }
}

double computeBreakpoint(Point* p1, Point* p2) {

    return 0;
}



