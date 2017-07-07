// Implementation based on http://www.cprogramming.com/tutorial/lesson18.html

#ifndef BTREE_H
#define BTREE_H

typedef struct node {
    void* val; // TODO figure out what this will actually hold
    node* left;
    node* right;
} node;

class BinaryTree {
    public:
        BinaryTree();
        ~BinaryTree();

        void insert(void* val);
        node* search(void* val);
        void destroyTree();

    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node* search(int key, node *leaf);

        node* root;
};

#endif /* BTREE_H */
