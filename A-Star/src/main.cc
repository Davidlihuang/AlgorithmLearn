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
    {0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
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
    
    
    
    Tile start(Point(0, 1));
    Tile target(Point(7, 1));
    Tile start1(Point(2, 1));
    Tile target1(Point(5, 1));

    // start1.setObstacle();
    // target1.setObstacle();
    
    //status = algo.run(start1, target1, false);
    graph.drawGraph();

    // start1.unsetObstacle();
    // target1.unsetObstacle();
    // start.setObstacle();
    // target.setObstacle();
   
    //status = algo.run(start, target, false);
    
    

    Tile start2(Point(1, 1));
    Tile target2(Point(3, 1));
    status = algo.run(start2, target2, false);


    Tile start3(Point(0, 2));
    Tile target3(Point(0, 8));
    //status = algo.run(start3, target3, false);
    
    std::cout << "\n----result----" <<std::endl;
    graph.drawGraph();
    return 0; 
}