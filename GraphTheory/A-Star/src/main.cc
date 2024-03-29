#include "graph.h"
#include "astar.h"
#include <iostream>
#include <sys/time.h>

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
    
    struct timeval tv_start, tv_end;
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
    //graph.drawGraph();

    // start1.unsetObstacle();
    // target1.unsetObstacle();
    // start.setObstacle();
    // target.setObstacle();
   
    //status = algo.run(start, target, false);
    
    
    
    
    Tile start2(Point(1, 1));
    Tile target2(Point(9, 9));

    gettimeofday( &tv_start, (struct timezone*) NULL);
    status = algo.run(start2, target2, true);


    Tile start3(Point(0, 2));
    Tile target3(Point(9, 3));
    //status = algo.run(start3, target3, false);
    gettimeofday( &tv_end, (struct timezone*) NULL);        
    printf("\nRuntime: %ld usec \n", tv_end.tv_usec - tv_start.tv_usec);
    
    std::cout << "\n----result----" <<std::endl;
    graph.drawGraph();
    return 0; 
}