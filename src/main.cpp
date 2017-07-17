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

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top().pe->x << "," <<  q.top().pe->y << endl;

        q.pop();
    }
    std::cout << '\n';
}

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

    Point* d = new Point(20,20);
    Point* a = new Point(-1,-2);
    Point* c = new Point(3,-3);
    Point* b = new Point(2,3);

    Event* de = new Event;
    de->pe = d;
    de->type = PointE;

    Event* ae = new Event;
    ae->pe = a;
    ae->type = PointE;

    Event* ce = new Event;
    ce->pe = c;
    ce->type = PointE;

    Event* be = new Event;
    be->pe = b;
    be->type = PointE;


    /* Circle* d = Circle::computeCircumcircle(a,b,c); */
    /* cout << "center: " << d->center->x << "," << d->center->y << endl; */
    /* cout << */ 

    /* cout << a < b << endl; */
    std::priority_queue<Event,vector<Event>,CompareEvent> pq;

    pq.push(*de);
    pq.push(*ae);
    pq.push(*ce);
    pq.push(*be);

    print_queue(pq);






    /* VTree* v = new VTree; */
    /* v-> */
}

