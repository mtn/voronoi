#pragma once

#include "point.h"

class Line {
   public:
       double slope;
       double yIntercept;

       Line(Point* a, Point* b);
       Line(double slope, double intercept);
       Line(Point* a, double slope);

       double getY(double x);
       double getX(double y);
       double dist(Point* p);

       static Point* getIntersection(Line* l1, Line* l2);

};


