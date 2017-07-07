#include "lib/point.h"

#include <iostream>

int main() {
    Point a;
    Point b;

    a.x = 1;
    a.y = 1;

    b.x = 1;
    b.y = 4;

    std::cout << a.dist(b);
}
