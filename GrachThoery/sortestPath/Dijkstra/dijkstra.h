#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <limits>

using namespace std;


template <class T>
class Graph
{
    
public:
    Graph(bool direction = false)
    {
        hasDirection = direction;
    }
    ~Graph()
    {
        for (int i = 0; i < nodeNums; i++)
        {
            delete[] edgeMat[i];
        }
        delete[] edgeMat;
    }

    bool createAdjMatrix(int nodeNums)
    {
        if (nodeNums < 0)
            return false;

        // 创建二维邻接矩阵
        edgeMat = new T *[nodeNums];
        for (int i = 0; i < nodeNums; i++)
        {
            edgeMat[i] = new T[nodeNums];
        }

        if (edgeMat == nullptr || edgeMat[0] == nullptr)
            return false;

        for(int i=0; i< nodeNums; i++)
        {
            for(int j=0; j< nodeNums; j++)
            {
                edgeMat[i][j] = edgeMat[j][i] = inf;
            }
        }
        return true;
    }
    bool initGraph(const std::string &fileName) // 文件输入
    {
        ifstream infile(fileName);
        if (!infile.is_open())
        {
            return false;
        }

        infile >> nodeNums >> edgeNums;
        if (nodeNums < 0 || edgeNums < 0)
            return false;

        if (createAdjMatrix(nodeNums) == false)
            return false;

        int a, b;
        T w;
        for (int i = 0; i < edgeNums; i++)
        {
            infile >> a >> b >> w;
            if ((a < 0 || a > nodeNums) || (b < 0 || b > nodeNums))
            {
                infile.close();
                return false;
            }

            edgeMat[a][b] = w;

            if (hasDirection == false)
            {
                edgeMat[b][a] = w;
            }
        }
        infile.close();
    }
    bool initGraph()
    {
        int n, m;

        std::cout << "输入节点数与边数：";
        std::cin >> n >> m;
        if (n < 0 || m < 0)
            return false;

        nodeNums = n;
        edgeNums = m;
        if (false == createAdjMatrix(n))
            return false;

        int a, b;
        T w;
        std::cout << "输入边,如a b w" << std::endl;
        for (int i = 0; i < m; i++)
        {
            std::cin >> a >> b >> w;
            if ((a < 0 || a > n) || (b < 0 || b > n))
                return false;
            edgeMat[a][b] = w;

            if (hasDirection == false)
            {
                edgeMat[b][a] = w;
            }
        }

    } // 标准输入

    const int getNodeNums() const { return nodeNums; };
    const int getEdgeNums() const { return edgeNums; };
    T **getAdjMatrix() { return edgeMat; };
    const T getEdgesWeight(int va, int vb)
    {
        if (false == (isVertexValid(va) && isVertexValid(vb)))
        {
            return -1;
        }
        return edgeMat[va][vb];
    }

    friend ostream &operator<<(ostream &os, Graph<T> &graph)
    {
        os << "{\n";
        auto nodeNums = graph.getNodeNums();
        auto edgeMat = graph.getAdjMatrix();
        for (int i = 0; i < nodeNums; i++)
        {
            os << "\t[ ";
            for (int j = 0; j < nodeNums; j++)
            {
                auto curretElem = edgeMat[i][j];
                os << setw(4) << curretElem << ", ";
            }
            os << "]\n";
        }
        os << "}\n";
        return os;
    }

private:
    bool isVertexValid(int v)
    {
        if (v < 0 || v > nodeNums)
            return false;
        return true;
    }

public:
    const T inf = std::numeric_limits<T>::infinity();
    
private:
    bool hasDirection;
    int nodeNums;
    int edgeNums;
    T **edgeMat;
    
};

template <class T, class Tw>
class Dijkstra
{
private:
    struct flagTab
    {
        bool isToken = false;
        Tw   dist = Tw();
        int  path = -1;
    };

public:
    explicit Dijkstra(T *g = nullptr)
    {
        if (g != nullptr)
        {
            graph = g;
            nodeNums = graph->getNodeNums();
            table.reserve(nodeNums);
        }
    }
    bool run(int startV);
private:
    const Tw getAdjEdgeWeight(int va, int vb) const
    {
        Tw w = graph->getEdgesWeight(va, vb);
    }
    const int getNodeNums() const {return graph->getNodeNums();}

private:
    T                 *graph;
    int               nodeNums;
    vector<flagTab>   table;
};
template<class T, class Tw>
bool Dijkstra<T, Tw>::run(int startV)
{
    for(int i=0; i< nodeNums; i++)
    {
        flagTab& tab = table[i];
        tab.dist     = graph->getAdjEdgeWeight(startV, i);
        if(i != startV && tab.dist) 
        {
            tab.path = startV;
        }
        else
        {
            tab.path = -1;
        }
    }
}
#endif