#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Node;
class Tile;

class Edge
{
public:
    int capacity;
    bool blocked;
    int cost;
    int idx;
    Node *src, *tar;

    Edge(Node *n1, Node *n2, int e_cost, vector<Edge *> &vec_edge);
    void blockEdge()
    {
        this->capacity = 0;
        this->blocked = true;
    }
};

#endif