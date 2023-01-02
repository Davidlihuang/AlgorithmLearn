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
    bool run(const Tile& start, const Tile& end);
    void getPath();
private:
    Astar();
    Astar(const Astar&);
    Astar& operator=(const Astar&);
    Astar& operator=(const Astar&) const;
    
private:    
    Graph* graph;
    priority_queue<Tile*> openTable;
    unordered_set<Tile*>  closeTable;
    vector<Tile*> Path;
};