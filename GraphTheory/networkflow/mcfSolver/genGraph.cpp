#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <sys/time.h>
#include "mcf.h"
#include "grid.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// launching point;
//
////////////////////////////////////////////////////////////////////////////////
int main()
{
    Graph graph(100,100,1,0);

    vector<Net> netlist;
    Net net1, net2, net3;
    Coord p1(0,0); Coord p2(2,3);
    net1.pins.push_back(p1);
    net1.pins.push_back(p2);

    Coord p3(50,10); Coord p4(20,23);
    net2.pins.push_back(p3);
    net2.pins.push_back(p4);

    Coord p5(10,0); Coord p6(30,50);
    net3.pins.push_back(p5);
    net3.pins.push_back(p6);

    netlist.push_back(net1);
    netlist.push_back(net2);
    netlist.push_back(net3);

    graph.initNetlist(netlist);
    graph.run();


}

