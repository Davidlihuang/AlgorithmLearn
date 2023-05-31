#include "graph.h"
#include <iostream>
#include "bidirectionAstar.h"
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

    Tile start3(Point(1, 1));
    Tile target3(Point(9, 9));
    
    

    BidirectAstar balgo(&graph);

    
    gettimeofday( &tv_start, (struct timezone*) NULL);
    auto status = balgo.run(start3, target3, true);
    
    gettimeofday( &tv_end, (struct timezone*) NULL);        
    printf("\nRuntime: %ld usec \n", tv_end.tv_usec - tv_start.tv_usec);

    std::cout << "\n----result----" <<std::endl;
    graph.drawGraph();
    return 0; 
}