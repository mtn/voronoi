#include "lib/line.h"
#include "lib/point.h"
#include "lib/circle.h"
#include "lib/voronoi.h"
#include "lib/dcel.h"
#include "lib/graphics.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <utility>
#include <queue>
#include <set>

using namespace std;

double sweeplineY; // This might only be safe to update on sevents and after cevents

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        if(q.top()->type == PointE)
            std::cout << q.top()->se->x << "," <<  q.top()->se->y << endl;
        else
            std::cout << "circleevent" << endl;

        q.pop();
    }
    std::cout << '\n';
}


int main(int argc, char** argv) {

    // TODO Implement a bounded pq that supports random deletion
    std::priority_queue<Event*,std::vector<Event*>,CompareEvent> pq;

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
            tmp->se = new Point(a,b);

            pq.push(tmp);
        }

        boundX = ceil(boundX);
        boundY = ceil(boundY);

        fs.close();
    } else { // Generates the points and stores them in t.in for further testing
        boundX = boundY = 1;

    }

    /* Graphics* g = new Graphics; */
    /* if(!g->init()) { */
    /*     cout << "Graphics initialization failed!" << endl; */
    /* } */

    /* g->close(); */

    // To work around the first insertion edgecase, the first breakpoint is manually
    // constructed and then inserted into the set
    bool first = true;
    int count = 0;
    BLSet beachline;
    Beachline* bl = new Beachline;
    Event *e1, *e2;
    while(!pq.empty()) {
        e1 = pq.top();
        pq.pop();
        if(count < 2) {
            // TODO handle degenerate case where the first two sites have the same y
            e2 = pq.top();
            pq.pop();

            // A circle event cannot occur within the first two events, so we don't
            // have to check event types
            bl->insertBreakpoint(e1,e2);
            bl->insertBreakpoint(e2,e1);

            count++;
            /* first = false; */
        }

        e2 = nullptr;
    }

    for (BLSet::iterator it=bl->set.begin(); it!=bl->set.end(); ++it)
        std::cout << ' ' << *it;




    /* Breakpoint b1 = std::make_pair(new Point(2,1),new Point(3,2)); */
    /* Breakpoint b2 = std::make_pair(new Point(3,2),new Point(2,1)); */
    /* cout << computeIntersection(b1,0.5) << endl; */
    /* cout << computeIntersection(b2,0.5) << endl; */

    /* CircleEvent* ce = new CircleEvent; */
    /* ce->c = new Circle(new Point(0.5,0.5),0.5); */
    /* Event* e = new Event; */
    /* e->ce = ce; */
    /* e->type = CircleE; */
    /* pq.push(e); */


    /* Breakpoint p(new Point(1,2),new Point(2,3)); */

    /* std::cout << p.first << "," << p.second << endl; */

    /* print_queue(pq); */



}

