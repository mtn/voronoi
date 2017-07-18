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

    if(argc > 1) {
        ifstream fs(argv[1]);
        double a, b;
        Event* tmp;

        while(fs >> a >> b){
            tmp = new Event;
            tmp->type = PointE;
            tmp->pe = new Point(a,b);

            pq.push(tmp);
        }

        fs.close();
    } else { // Generate the points

    }

    /* CircleEvent* ce = new CircleEvent; */
    /* ce->c = new Circle(new Point(0.5,0.5),0.5); */
    /* Event* e = new Event; */
    /* e->ce = ce; */
    /* e->type = CircleE; */
    /* pq.push(e); */

    print_queue(pq);



}

