#include "lib/line.h"
#include "lib/point.h"
#include "lib/parabola.h"
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

    Point* a = new Point(1,2);
    Point* b = new Point(3,4);
    Point* c = new Point(5,1);

    Circle* d = Circle::computeCircumcircle(a,b,c);
    if(d != nullptr) {
        cout << "it was not null" << endl;
    } else {
        cout << "it was null" << endl;
    }




    /* VTree* v = new VTree; */
    /* v-> */
}

