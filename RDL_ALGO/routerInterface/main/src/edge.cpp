#include "node.h"
#include "edge.h"
#include "tile.h"

Edge::Edge(Node *n1, Node *n2, int _cost, vector<Edge *> &vec_edge)
{
    this->cost = _cost;
    this->src = n1;
    this->tar = n2;
    this->capacity = 1;
    this->blocked = false;
    //n1->push_to_edge(this);
    //n2->push_to_edge(this);
    this->idx = vec_edge.size();
    vec_edge.push_back(this);
}