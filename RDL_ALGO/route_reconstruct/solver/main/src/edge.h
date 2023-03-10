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
/**
 * @brief  描述一条边
 * 
 */
class Edge
{
public:
    int capacity; //容量
    bool blocked; //
    int cost; //代价
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