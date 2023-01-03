#include "graph.h"
#include "astar.h"
#include <iostream>

using namespace std;
const vector<vector<int>> maze{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
int main()
{
    // init graph
    Graph graph(maze);
    std::cout << "----map----" <<std::endl;
    graph.drawGraph();
    
    //< path
    Astar algo(&graph);
    bool status;
   
    Tile start1(Point(0, 0));
    Tile target1(Point(9, 9));
    //status = algo.run(start1, target1, false);

    Tile start(Point(3, 4));
    Tile target(Point(5, 6));
    //status = algo.run(start, target, false);
    

    Tile start2(Point(7, 2));
    Tile target2(Point(0, 8));
    status = algo.run(start2, target2, true);


    Tile start3(Point(6, 1));
    Tile target3(Point(4, 1));
    //status = algo.run(start3, target3, false);
    
    std::cout << "\n----result----" <<std::endl;
    graph.drawGraph();
    return 0; 
}