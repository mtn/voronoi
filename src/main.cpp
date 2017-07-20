#include "lib/line.h"
#include "lib/point.h"
#include "lib/circle.h"
#include "lib/voronoi.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <utility>
#include <queue>
#include <set>

using namespace std;


template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        if(q.top()->type == PointE)
            std::cout << q.top()->pe->x << "," <<  q.top()->pe->y << endl;
        else
            std::cout << "circleevent" << endl;

        q.pop();
    }
    std::cout << '\n';
}

int main(int argc, char** argv) {

    std::priority_queue<Event*,vector<Event*>,CompareEvent> pq;

    double boundX = 0, boundY = 0;
    if(argc > 1) {
        ifstream fs(argv[1]);
        double a, b;
        Event* tmp;

        while(fs >> a >> b){
            if(a > boundX) boundX = a;
            if(b > boundY) boundY = b;

            tmp = new Event;
            tmp->type = PointE;
            tmp->pe = new Point(a,b);

            pq.push(tmp);
        }

        boundX = ceil(boundX);
        boundY = ceil(boundY); // TODO scale/coerce these values to 1x1

        fs.close();
    } else { // Generate the points
        boundX = boundY = 1;

    }

    /* CircleEvent* ce = new CircleEvent; */
    /* ce->c = new Circle(new Point(0.5,0.5),0.5); */
    /* Event* e = new Event; */
    /* e->ce = ce; */
    /* e->type = CircleE; */
    /* pq.push(e); */

    /* set<Point,ComparePoint> beachline; */

    /* Breakpoint p(new Point(1,2),new Point(2,3)); */

    /* std::cout << p.first << "," << p.second << endl; */

    /* print_queue(pq); */



}

