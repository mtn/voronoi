// Implementation based on http://www.cprogramming.com/tutorial/lesson18.html

#ifndef PARABOLA_H
#define PARABOLA_H

#include "point.h"

typedef struct node {
    Point* value; // TODO figure out what this will actually hold
    node* left;
    node* right;
} node;

class BinaryTree {
    public:
        BinaryTree();
        ~BinaryTree();

        void insert(Point* key);
        node* search(Point* key);
        void destroyTree();

    private:
        void destroy_tree(node *leaf);
        void insert(Point* key, node *leaf);
        node* search(Point* key, node *leaf);

        node* root;
};

#endif /* PARABOLA_H */
