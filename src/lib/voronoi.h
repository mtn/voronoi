// Defines internal representation of Voronoi Diagram

#ifndef VORONOI_H
#define VORONOI_H

class Point;

class Node {
    public:
        Node* parent;
        Node* left;
        Node* right;

        double breakpoint;
        Point* site;

        // In-order next and prev
        Node* succ();
        Node* pred();

    private:
        Node* getMinimum(); // Returns minimal node in subtree
        Node* getMaximum(); // Returns maximal node in subtree
        Node* findSuccessorInAncestors();
        Node* findPredecessorInAncestors();
};

// Implementation based on http://www.cprogramming.com/tutorial/lesson18.html
class VTree {
    public:
        VTree();
        ~VTree();

        void destroyTree();
        void insert(double x); // Receieves site's x coord as argument

    private:
        void destroyTree(Node* leaf);
        void insert(double x, Node* leaf);

        static double computeBreakpoint(Point* p1, Point* p2);

        Node* root;
        Node* firstLeaf;
};


#endif /* VORONOI_H */

