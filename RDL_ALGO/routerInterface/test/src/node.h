#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Tile;
class Edge;

class Node
{
public:
    int capacity;
    int idx;
    //int cost;
    //int RDL_num;
    vector<Edge *> edges;
    Tile *parent_tile;

    Node(Tile *tile_init, vector<Node *> &vec_node) : parent_tile(tile_init)
    {
        this->idx = vec_node.size();
        vec_node.push_back(this);
    }

    Node() {}

    void push_to_edge(Edge *edge);
    Tile *get_tile() { return this->parent_tile; }
    void print() { cout << "hello world" << endl; }
};

#endif