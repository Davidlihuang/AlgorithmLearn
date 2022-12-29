#pragma once
#include "graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>

using namespace std;

class Astar
{
public:
    explicit Astar(Graph* g):graph(g){};
    void run(const Point& start, const Point& end)
    {
        
    };

private:
    Astar(const Astar&);
    Astar& operator=(const Astar&);
    Astar& operator=(const Astar&) const;
    Astar& operator=(const Astar&&);


private:    
    Graph* graph;
    priority_queue<Point*> openTable;
    unordered_set<Point*>  closeTable;
    vector<Point*> Path;
};