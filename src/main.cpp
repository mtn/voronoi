#include "lib/line.h"
#include "lib/point.h"
#include "lib/parabola.h"
#include "lib/voronoi.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <queue>
#include <list>

using namespace std;

int main(int argc, char** argv) {
    if(argc > 1) {
        ifstream file (argv[1]);
        int numSites;
        file >> numSites;

        for(int i = 0; i < numSites; i++) {

        }

        cout << numSites << endl;

        file.close();
        return 0;
    }
}

