#include "dijkstra.h"
#include <iostream>

using namespace std;

int main()
{
    Graph<int> g(false);
    g.initGraph("graph.txt");
    cout << g << endl;

    //g.initGraph();
    //cout << g << endl;
    return 0;
}