#ifndef __GRID_H_
#define __GRID_H_
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Coord3
{
    int x;
    int y;
    int h;
};

struct Coord
{
    int x;
    int y;
    Coord(){}
    Coord(int i, int j){x = i;y  =j;}
    bool operator==(const Coord& p1)
    {
        if(p1.x == this->x && p1.y == this->y)
        {
            return true;
        }
        return false;
    }
};
struct Node
{
    int idx;
    Coord locate;
    bool  inDirect;
    Node(){}
    Node(int id, const Coord& l){
        locate  = l;
        idx = id;
        inDirect = false;
    }
};


struct Edge
{
    int idx;
    int src;
    int dst;
    Node* srcNode;
    Node* dstNode;
    double capacity;
    double cost;
    Edge(){}
    Edge(Node* in, Node* out)
    {
        srcNode = in;
        dstNode = out;
    }
};



struct Net
{
    int idx;
    string name;
    vector<Coord> pins;
};

struct netNodes
{
    int pin1;
    int pin2;
    double supply;
    netNodes(){};
    netNodes(int p1, int p2, double k)
    {
        pin1 = p1;
        pin2 = p2;
        supply =  k;
    };
};

class Graph
{
private:    
    int width;
    int height;
    int wireSpace;
    int wireWidth;
    vector<Node*> nodes;
    vector<Edge*> arcs;
    vector<netNodes> netListNodes;

    vector<Net> netList;

public:
    void loadDesign(const std::string& filename);
    void genNetworkFlowGraph();
    void exportGraphToTxt(const std::string& filename);
    void initNetlist(const vector<Net>& nl)
    {
        this->netList = nl;
    }
    void run();

private:
    void mapNetListToGraph();
    vector<Node*> findPinNodes(const Coord& p1);

public:    
    Graph(){}
    Graph(int FW, int FH, int ww, int ws)
    {
        width = FW;
        height = FH;
        wireSpace = ws;
        wireWidth = ww;
    }
    ~Graph(){
        for(size_t i =0; i< nodes.size(); i++)
        {
            delete nodes[i];
            nodes[i] = nullptr;
        }

        for(size_t j=0; j< arcs.size(); j++)
        {
            delete arcs[j];
            arcs[j] = nullptr;
        }
    }
};

#endif