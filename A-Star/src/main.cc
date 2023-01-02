#include "graph.h"
#include "astar.h"
#include <iostream>

using namespace std;
const vector<vector<int>> maze{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
int main()
{
    // init graph
    Graph graph(maze);
    graph.drawGraph();
    
    //< path
    Tile start(Point(0, 0), 0, 0);
    Tile target(Point(9, 9), 9, 9);
    Astar algo(&graph);
    bool status = algo.run(&start, &target);
    
    cout <<"status:" << status <<endl;

    return 0; 
}