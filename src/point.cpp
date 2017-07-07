#include "lib/point.h"

#include <math.h>

double Point::dist(Point a) {
    return sqrt(pow(a.x-this->x,2) + pow(a.y-this->y,2));
}
