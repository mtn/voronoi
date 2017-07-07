#ifndef LINE_H
#define LINE_H

#include "point.h"

class Line {
   public:
       double slope;
       double yIntercept;

       Line(Point* a, Point* b);
       Line(double slope, double intercept);


};

#endif /* LINE_H */

