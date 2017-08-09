#pragma once

#include "point.h"

// TODO make an interface based on how these were used
// eg. Creating a sibling should happen in a function rather than e.s.s=e, etc.

struct vert {
    double x;
    double y;

    struct edge* edge; // list of incident half-edges
};

struct edge {
    struct vert* start; // vertex the edge emanates from
    struct vert* end;

    struct face* face; // possibly null

    struct edge* sibling;

    struct edge* next; // clockwise next
    struct edge* prev;
};

struct face {
    struct edge* edge;
};

typedef struct vert DCEL_Vert;
typedef struct edge DCEL_Edge;
typedef struct face DCEL_Face;

