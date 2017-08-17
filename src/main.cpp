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

    /* bool first = true; */
    Event *e1, *e2;
    Beachline* bl;
    /* while(!eq.empty()) { */
        e1 = eq.top();
        eq.pop();

    /*     // TODO handle degenerate case where the first two sites have the same y */
    /*     if(first) { */
            e2 = eq.top();
            eq.pop();

            bl = new Beachline(e1,e2);

            BLNode* min = bl->findMin();

            cout << "e1 " << e1->se->toString() << endl;
            cout << "e2 " << e2->se->toString() << endl;

            sweeplineY = 5;
            if(min->getBreakpoint()->first == e1->se) {
                cout << "e1 was first" << endl;
            }else{cout << "e2 was first" << endl;}
            cout << "intersection " << min->computeIntersection(sweeplineY) << endl;
            BLNode* succ = bl->getSuccessor(min);
            cout << "succ " << succ->computeIntersection(sweeplineY) << endl;
            BLNode* oldmin = min;

            BLNode* oldsucc = succ;
            Point* p = new Point(3,5);
            bl->handleSiteEvent(p);
            min = bl->findMin();
            if(min == oldmin) {
                cout << "min stayed the saem" << endl;
            } else {
                cout << "min changed" << endl;
            }
            succ = bl->getSuccessor(min);
            if(succ == oldsucc) {
                cout << "succstayed the saem" << endl;
            } else {
                sweeplineY = 6;
                cout << "succchanged" << endl;
                cout << min->getBreakpoint()->first->toString() << " " << min->getBreakpoint()->second->toString() << " " << min->computeIntersection(sweeplineY) << endl;
                cout << succ->getBreakpoint()->first->toString() << " " << succ->getBreakpoint()->second->toString() << " " << succ->computeIntersection(sweeplineY) << endl;
                succ = bl->getSuccessor(succ);
                cout << succ->getBreakpoint()->first->toString() << " " << succ->getBreakpoint()->second->toString() << " " << succ->computeIntersection(sweeplineY) << endl;
                succ = bl->getSuccessor(succ);
                if(succ) cout << succ->getBreakpoint()->first->toString() << " " << succ->getBreakpoint()->second->toString() << " " << succ->computeIntersection(sweeplineY) << endl;
            }

            /* cout << min->computeIntersection(sweeplineY) << endl; */

    /*         e2 = nullptr; */
    /*         first = false; */
    /*     } else { */
    /*         /1* bl->handleSiteEvent(e1->se); *1/ */
    /*         bl->insert(e1,e2); */
    /*         bl->insert(e2,e1); */
    /*     } */
    /*     /1*     if(count < 10) { *1/ */
    /*     /1*         count++; *1/ */
    /*     /1*         if(e1->type == SiteE) { *1/ */
    /*     /1*             bl->handleSiteEvent(e1->se); *1/ */
    /*     /1*         } else { *1/ */
    /*     /1*             bl->handleCircleEvent(e1->ce); *1/ */
    /*     /1*         } *1/ */
    /*     /1*     } *1/ */
    /*     /1* } *1/ */

    /* } */

    /* BLNode* min = bl->findMin(); */
    /* cout << "first " << min << endl; */
    /* /1* /2* cout << min->getBreakpoint()->first->x << "," << min->getBreakpoint()->first->y << endl; *2/ *1/ */
    /* /1* /2* cout << min->getBreakpoint()->second->x << "," << min->getBreakpoint()->second->y << endl; *2/ *1/ */
    /* cout << "first removal " << endl; */
    /* bl->remove(min); */
    /* cout << "first removal finished" << endl; */
    /* /1* /2* cout << "removed" << endl; *2/ *1/ */
    /* min = bl->findMin(); */
    /* /1* cout << "second " <<  min << endl; *1/ */
    /* /1* bl->remove(min); *1/ */
    /* /1* /2* cout << "New min " << min << endl; *2/ *1/ */
    /* /1* /2* cout << min->getBreakpoint()->first->x << "," << min->getBreakpoint()->first->y << endl; *2/ *1/ */
    /* /1* /2* cout << min->getBreakpoint()->second->x << "," << min->getBreakpoint()->second->y << endl; *2/ *1/ */
    /* /1* /2* cout << "removing " << min << endl; *2/ *1/ */
    /* /1* bl->remove(min); *1/ */
    /* /1* cout << min->getBreakpoint()->first->x << "," << min->getBreakpoint()->first->y << endl; *1/ */
    /* cout << min->getBreakpoint()->second->x << "," << min->getBreakpoint()->second->y << endl; */
    /* /1* cout << "removing " << min << endl; *1/ */
    /* bl->remove(min); */
    /* /1* cout << "removing " << min << endl; *1/ */
    /* bl->remove(min); */
    /* cout << min->getBreakpoint()->first->x << "," << min->getBreakpoint()->first->y << endl; */
    /* cout << min->getBreakpoint()->second->x << "," << min->getBreakpoint()->second->y << endl; */
    /* /1* cout << "removing " << min << endl; *1/ */
    /* bl->remove(min); */
    /* cout << min->getBreakpoint()->first->x << "," << min->getBreakpoint()->first->y << endl; */
    /* cout << min->getBreakpoint()->second->x << "," << min->getBreakpoint()->second->y << endl; */
    /* /1* cout << "removing " << min << endl; *1/ */
    /* bl->remove(min); */
    /* cout << "removed" << endl; */


    /* sweeplineY = boundY; */
    /* while(min) { */
    /*     cout << min->computeIntersection(sweeplineY) << endl; */
    /*     min = bl->getSuccessor(min); */
    /* } */
    /* cout << endl; */
    /* cout << count << endl; */
}

