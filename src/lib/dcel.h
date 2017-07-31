#ifndef DCEL_H
#define DCEL_H

#include "point.h"

enum Color { Green, Blue, Red, Yellow };

struct vert {
    double x;
    double y;

    struct edge* edge;
};

struct edge {
    struct vert* vert; // vertex the edge emanates from
    struct face* face; // possibly null

    struct edge* sibling;
    struct edge* next; // clockwise next
};

struct face {
    struct edge* edge;
    Color c;
};

typedef struct vert DCEL_Vert;
typedef struct edge DCEL_Edge;
typedef struct face DCEL_Face;



#endif /* DCEL_H */
