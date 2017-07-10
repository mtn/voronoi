#include "lib/voronoi.h"
#include "lib/point.h"

#include <cstddef>
#include <limits>


Node* Node::succ() {
    if(this->right != NULL) {
        return this->right->getMinimum();
    } else {
        return this->findSuccessorInAncestors();
    }
}

Node* Node::getMinimum() {
    if(this->left != NULL) {
        return this->left->getMinimum();
    } else {
        return this;
    }
}

Node* Node::findSuccessorInAncestors() {
    if(this->parent && this == this->parent->right) {
        return this->parent->findSuccessorInAncestors();
    } else {
        return this;
    }
}

Node* Node::pred() {
    if(this->right != NULL) {
        return this->left->getMaximum();
    } else {
        return this->findPredecessorInAncestors();
    }
}

Node* Node::getMaximum() {
    if(this->right != NULL) {
        return this->right->getMaximum();
    } else {
        return this;
    }
}

Node* Node::findPredecessorInAncestors() {
    if(this->parent && this == this->parent->left) {
        return this->parent->findPredecessorInAncestors();
    } else {
        return this;
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

void VTree::destroyTree(Node* leaf) {
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
        root = new Node;
        root->breakpoint = x;
        root->left = NULL;
        root->right = NULL;
    }
}

void VTree::insert(double breakpoint, Node* node) {
    if(breakpoint < node->breakpoint) {
        if(node->left != NULL) {
            insert(breakpoint,node->left);
        } else {
            node->left = new Node;
            node->left->breakpoint = breakpoint;
            node->left->left = NULL;
            node->left->right= NULL;
        }
    } else {
        if(node->right != NULL) {
            insert(breakpoint,node->right);
        } else {
            node->right = new Node;
            node->right->breakpoint = breakpoint;
            node->right->left = NULL;
            node->right->right= NULL;
        }
    }
}

double computeBreakpoint(Point* p1, Point* p2) {

    return 0;
}



