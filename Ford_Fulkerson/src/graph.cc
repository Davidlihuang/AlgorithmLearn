#include "graph.h"
using namespace networkflow;
Graph::Graph() : 
    nodeNums(0), edgeNums(0) 
{ 
    graph.clear();
    maxflow = 0;
 }

// 标准输入流读入
void Graph::readGraphFromStdInputStream(int nodeNum)
{

}

// 文件读入流
void Graph::readGraphFromFile(const string &filename)
{
    
}

// 默认图
void Graph::constructDefaultGraph()
{
    vector<vector<int>> g = {
        {0, 8, 4, 0, 0, 0},
        {0, 0, 0, 2, 2, 0},
        {0, 4, 0, 1, 4, 0},
        {0, 0, 0, 0, 6, 9},
        {0, 0, 0, 0, 0, 7},
        {0, 0, 0, 0, 0, 0},
    };

    // 构造图
    this->nodeNums = g.size();
    this->edgeNums = 0;
    for (int i = 0; i < g.size(); i++)
    {
        vector<ArcType *> tmpArc;
        for (int j = 0; j < g[0].size(); j++)
        {

            ArcType *arc = nullptr;
            if (g[i][j] != 0)
            {
                arc = new ArcType(g[i][j], 0);
                arc->source = i;
                arc->target = j;
                arc->direction = 0;
                this->edgeNums += 1;
            }
            tmpArc.push_back(arc);
        }
        this->graph.push_back(tmpArc);
    }

    cout << "graph: nodes=" << this->graph.size() << endl;
}

// 获取图的邻接节点
vector<ArcType *> Graph::getNeibor(int nodeIndex)
{
    vector<ArcType *> node;
    if (nodeIndex < 0 || nodeIndex >= this->nodeNums)
        return node;

    for (int i = 0; i < this->nodeNums; i++)
    {
        if (this->graph[nodeIndex][i] != nullptr)
        {
            node.push_back(this->graph[nodeIndex][i]);
        }
    }
    return node;
}

// 获取正向弧
vector<ArcType *> Graph::getPositiveEdge(int nodeIndex)
{
    return getNeibor(nodeIndex);
}

// 获取反向弧
vector<ArcType *> Graph::getNegtiveEdge(int nodeIndex)
{
    vector<ArcType *> node;
    if (nodeIndex < 0 || nodeIndex >= this->nodeNums)
        return node;

    for (int i = 0; i < this->nodeNums; i++)
    {
        if (this->graph[i][nodeIndex] != nullptr)
        {
            node.push_back(this->graph[i][nodeIndex]);
        }
    }
    return node;
}

int Graph::getNodesNums() const
{
    return this->nodeNums;
}

int Graph::getEdgeNums() const
{
    return this->edgeNums;
}
