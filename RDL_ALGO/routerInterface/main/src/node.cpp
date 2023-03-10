
#include "edge.h"
#include "node.h"
#include "tile.h"

void Node::push_to_edge(Edge *edge)
{
    this->edges.push_back(edge);
}
