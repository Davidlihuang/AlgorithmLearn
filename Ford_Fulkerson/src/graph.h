#ifndef GRAPH_H_
#define GRAPH_H_
#include <iostream>
#include <vector>
#include <string>
#include <queue>
namespace networkflow
{

    using namespace std;
    /**
     * @brief 图中边的表示
     * 
     */
    struct ArcType
    {
        ArcType(int c=0, int f=0)
        : cap(c), flow(f), source(-1), target(-1)
        {
            
        }

        int cap;       // 容量
        int flow;      // 当前流
        int source;    // 源点
        int target;    // 目标点
        int direction; // 正向弧：0， 反向弧：1
    };

    /**
     * @brief 图的表示抽象类
     * 
     */
    class Graph
    {
    public:
        Graph();

        //标准输入流读入
        void readGraphFromStdInputStream(int nodeNum);

        //文件读入流
        void readGraphFromFile(const string& filename);

        //默认图
        void constructDefaultGraph();
        

        //获取图的邻接节点
        vector<ArcType*> getNeibor(int nodeIndex);
        

        //获取正向弧
        vector<ArcType*> getPositiveEdge(int nodeIndex);
        

        //获取反向弧
        vector<ArcType*> getNegtiveEdge(int nodeIndex);
        

        int getNodesNums() const;
        
        int getEdgeNums() const;
       
        bool getEdge(int s, int t, ArcType** arc)
        {
            if(s< 0 || t < 0) return false;
            if(s >= nodeNums || t>=nodeNums) return false;
            *arc = graph[s][t];
            return true;
        }

        int getMaxFlow()
        {
            for(int i=0; i< nodeNums; i++)
            {
                for(int j =0; j< nodeNums; j++)
                {
                    if(i == 0 && graph[i][j] != nullptr) 
                    {
                        this->maxflow += graph[i][j]->flow;
                    }
                    if(graph[i][j] != nullptr)
                    {
                        cout << "edage: (" << i << "->" << j << ")  flow: " << graph[i][j]->flow<< endl; 
                    }
                }
            }
            return this->maxflow;
        }
    private:
        int maxflow;
        int nodeNums;
        int edgeNums;
        vector<vector<ArcType*>> graph; //邻接矩阵表示
    };

}; // end namespace networkflow

#endif