#pragma once
#include "graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <functional>
#include <deque>

using namespace std;
class Lee
{
public:    
    Lee(Graph* g=nullptr): 
        graph(g)
    {};

    bool run(const Tile& start, const Tile& target, bool direction);
    

private:
    Lee(const Lee& );
    Lee(const Lee&&);
    Lee& operator=(Lee&) ;
    Lee& operator=(const Lee&) const;

private:

    Graph * graph;
    deque<Tile*> que;
    vector<Tile*> path;
};
