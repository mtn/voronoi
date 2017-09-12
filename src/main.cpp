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

using namespace std;

double sweeplineY;
EventQueue eq;

template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        if(q.top()->type == SiteE)
            std::cout << q.top()->se->x << "," <<  q.top()->se->y << endl;
        else
            std::cout << "circleevent" << endl;

        q.pop();

    std::cout << '\n';
    }
}


int main(int argc, char** argv) {
    // TODO Implement a bounded pq that supports random deletion
    cout << "hi" << endl;

    double boundX = 0, boundY = 0;
    if(argc > 1) {
        ifstream fs(argv[1]);

        if(!fs) {
            cerr << "File was not opened successfully! Exiting." << endl;
            return 1;
        }

        double a, b;
        Event* tmp;

        while(fs >> a >> b){
            if(a > boundX) boundX = a;
            if(b > boundY) boundY = b;

            tmp = new Event(new Point(a,b));
            eq.push(tmp);
        }

        boundX = ceil(boundX);
        boundY = ceil(boundY);

        fs.close();
    } else { // Generates the points and stores them in last.in for further testing
        boundX = boundY = 1;
        return 0;
    }

    /* Graphics* g = new Graphics; */
    /* if(!g->init()) { */
    /*     cout << "Graphics initialization failed!" << endl; */
    /* } */

    /* g->close(); */

    bool first = true;
    Event *e1, *e2;
    Beachline* bl;
    while(!eq.empty()) {
        e1 = eq.top();
        eq.pop();

        if(first) {
            e2 = eq.top();
            eq.pop();

            bl = new Beachline(e1,e2);

            e2 = nullptr;
            first = false;
        } else {
            bl->handleEvent(e1);
        }

    }

    sweeplineY = 5.01;
    BLNode* min = bl->findMin();
    while(min) {
        cout << "int " << min->computeIntersection(sweeplineY) << endl;
        bl->remove(min);
        min = bl->findMin();
    }
}
