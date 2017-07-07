#include "lib/binaryTree.h"

#include <stddef.h>

BinaryTree::BinaryTree() {
    root = NULL;
}

void BinaryTree::insert(void* val) {

}

node* BinaryTree::search(void* val) {

}

void BinaryTree::destroy_tree(node *leaf) {
    if(leaf!=NULL) {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
    }
}

