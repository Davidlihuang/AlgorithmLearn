#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include "graph.h"
#include "fordfulkerson.h"
using namespace std;
using namespace networkflow;

int main()
{
    Graph graph;
    graph.constructDefaultGraph();
    Fordfulkson flow(&graph);
    flow.run(0, 5);

    // auto ptEdge = graph.getPositiveEdge(1);
    // cout << "正向弧：" << endl;
    // for(int i=0; i< ptEdge.size();i++)
    // {
    //     cout << ptEdge[i]->source 
    //     << "->" << ptEdge[i]->target
    //     << ": (" << ptEdge[i]->cap
    //     << ", " << ptEdge[i]->flow
    //     << ")";
    //     cout << endl;
    // }
    
    // auto ntEdge = graph.getNegtiveEdge(1);
    // cout << "反向弧：" << endl;
    // for(int i=0; i< ntEdge.size();i++)
    // {
    //     cout << ntEdge[i]->source 
    //     << "->" << ntEdge[i]->target
    //     << ": (" << ntEdge[i]->cap
    //     << ", " << ntEdge[i]->flow
    //     << ")";
    //     cout << endl;
    // }

    return 0;
};