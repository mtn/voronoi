#include "lib/line.h"
#include "lib/point.h"
#include "lib/circle.h"
#include "lib/voronoi.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <list>

using namespace std;

int main(int argc, char** argv) {
    if(argc > 1) {
        ifstream fs (argv[1]);
        /* double a, b; */


        /* priority_queue<Point,vector<Point>> pq; */
        /* while(fs >> a >> b){ */
        /*     pq.push(Point(a,b)); */
        /*     cout << a << b << endl; */
        /* } */

        fs.close();
        return 0;
    } else { // Generate the points

    }

    Point* a = new Point(-1,-2);
    Point* b = new Point(2,3);
    Point* c = new Point(3,-3);

    Circle* d = Circle::computeCircumcircle(a,b,c);
    cout << "center: " << d->center->x << "," << d->center->y << endl;




    /* VTree* v = new VTree; */
    /* v-> */
}

