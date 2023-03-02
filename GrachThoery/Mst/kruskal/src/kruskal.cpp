#include <iostream>
#include <algorithm>
#include <vector>
#include "unionSet.h"
#define INFINITY INT32_MAX

using namespace std;
using VertexType = int;
using EdgeType = int;

class Edge
{
public:    
    int begin;
    int end;
    int weight;
    bool operator<(const Edge &a) const
    {
        return this->weight < a.weight;
    }
    Edge(int b, int e, int w) : begin(b), end(e), weight(w)
    {
    }

   friend ostream &operator<<(ostream &out, Edge &e);

};
ostream &operator<<(ostream &out, Edge &e)
{
    out << "( " << e.begin
        << ", " << e.end
        << " ): " << e.weight
        << " ";
    return out;
}
typedef struct
{
    vector<VertexType> vexs;
    vector<vector<EdgeType>> arc;
    int numVertexs, numEdges;
} MGraph;

void MinSpanTree_Kruskal(const MGraph &G)
{
    // 从图中获取所有边的集合
    vector<Edge> edges;
    vector<Edge> minSpanTree;

    for (int i = 0; i < G.numVertexs; i++)
    {
        for (int j = i + 1; j < G.numVertexs; j++)
        {
            if (G.arc[i][j] != INFINITY)
            {
                edges.push_back(Edge(i, j, G.arc[i][j]));
            }
        }
    }

    // 升序排列边集合
    sort(edges.begin(), edges.end());

    // 初始化并查集
    DisjointSet unionSet(G.numVertexs);

    // 寻找最下生成树
    for (int i = 0; i < edges.size(); i++)
    {
        auto e = edges[i];
        if (unionSet.unionTree(e.begin, e.end))
        {
            // 不在一个集合中，是生成树的一条边
            cout << "( " << e.begin
                 << ", " << e.end
                 << ", " << e.weight
                 << " )" << endl;
        }
        else
        {
            continue;
        }
    }
}

void MinSpanTree_Kruskal(vector<Edge> &edges, vector<Edge> &minSpanTree)
{
    if (edges.empty())
        return;
    minSpanTree.clear();

    // 升序排列边集合
    sort(edges.begin(), edges.end());

    // 初始化并查集
    DisjointSet unionSet(edges.size());

    // 寻找最下生成树
    for (int i = 0; i < edges.size(); i++)
    {
        auto e = edges[i];
        if (unionSet.unionTree(e.begin, e.end))
        {
            // 不连通，是生成树的一条边
            minSpanTree.push_back(e);
        }
        else
        {
            // 边的两个节点连通，构成环
            continue;
        }
    }
}

int main()
{
    MGraph G;
    G.numVertexs = 9; // 9个顶点
    G.numEdges = 15;  // 15条边
    for (int i = 0; i < G.numVertexs; i++)
        G.vexs.push_back(i);

    // 邻接矩阵
    G.arc = {
        {0, 10, INFINITY, INFINITY, INFINITY, 11, INFINITY, INFINITY, INFINITY},
        {10, 0, 18, INFINITY, INFINITY, INFINITY, 16, INFINITY, 12},
        {INFINITY, 18, 0, 22, INFINITY, INFINITY, INFINITY, INFINITY, 8},
        {INFINITY, INFINITY, 22, 0, 20, INFINITY, 24, 16, 21},
        {INFINITY, INFINITY, INFINITY, 20, 0, 26, INFINITY, 7, INFINITY},
        {11, INFINITY, INFINITY, INFINITY, 20, 0, 17, INFINITY, INFINITY},
        {INFINITY, 16, INFINITY, INFINITY, INFINITY, 17, 0, 19, INFINITY},
        {INFINITY, INFINITY, INFINITY, 16, 7, INFINITY, 19, 0, INFINITY},
        {INFINITY, 12, 8, 21, INFINITY, INFINITY, INFINITY, INFINITY, 0}};

    // MinSpanTree_Kruskal(G);

    vector<Edge> edges;
    vector<Edge> minSpanTree;
    for (int i = 0; i < G.numVertexs; i++)
    {
        for (int j = i + 1; j < G.numVertexs; j++)
        {
            if (G.arc[i][j] != INFINITY)
            {
                edges.push_back(Edge(i, j, G.arc[i][j]));
            }
        }
    }

    MinSpanTree_Kruskal(edges, minSpanTree);

    for (int i = 0; i < minSpanTree.size(); i++)
    {
        auto tmp = minSpanTree[i];
       cout << tmp << endl;
    }
    return 0;
}