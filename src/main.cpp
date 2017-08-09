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

#include <stdio.h>

using namespace std;

double sweeplineY; // This might only be safe to update on sevents and after cevents

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        if(q.top()->type == PointE)
            std::cout << q.top()->se->x << "," <<  q.top()->se->y << endl;
        else
            std::cout << "circleevent" << endl;

        q.pop();

    std::cout << '\n';
    }
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
        exit(0);
    }

    /* Graphics* g = new Graphics; */
    /* if(!g->init()) { */
    /*     cout << "Graphics initialization failed!" << endl; */
    /* } */

    /* g->close(); */

    int count = 0;
    bool first = true;
    Event *e1, *e2;
    Beachline* bl;
    while(!pq.empty()) {
        e1 = pq.top();
        pq.pop();
            // TODO handle degenerate case where the first two sites have the same y
        if(first) {
            e2 = pq.top();
            pq.pop();
            bl = new Beachline(e1,e2);

            e2 = nullptr;
            first = false;
        } else {
            if(count < 5) {
                count++;
                if(e1->type == PointE) {
                    bl->handleSiteEvent(e1->se);
                } else {
                    bl->handleCircleEvent(e1->ce);
                }
            }
        }

    }

/*     BLNode* min; */
/*     sweeplineY = boundY; */
/*     min = bl->findMin(); */
/*     while(min) { */
/*         cout << min->computeIntersection(sweeplineY) << endl; */
/*         min = bl->getSuccessor(min); */
/*     } */
/*     cout << endl; */




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




}

