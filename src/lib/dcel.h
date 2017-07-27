// Based on http://www.flipcode.com/archives/The_Half-Edge_Data_Structure.shtml

#ifndef DCEL_H
#define DCEL_H

enum Color { Green, Blue, Red, Yellow };

struct vert {
    double x;
    double y;

    struct edge* edge;
};

struct edge {
    struct vert* vert;
    struct face* face;
    struct edge* pair;
    struct edge* next;
};

struct  face {
    struct edge* edge;
    Color c;
};

typedef struct vert DCEL_Vert;
typedef struct edge DCEL_Edge;
typedef struct face DCEL_Face;




#endif /* DCEL_H */
