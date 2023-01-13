#ifndef FORDFULKSON_H_
#define FORDFULKSON_H_

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include "graph.h"

const int32_t inf = INT32_MAX;

namespace networkflow
{
    using namespace std;
    struct Labeltype
    {
        Labeltype(int f = -1, int p= -1, int a=0):
            flag(f),parent(p),alpha(a){};
        void set(int f = -1, int p= -1, int a=0)
        {
            flag = f;
            parent = p;
            alpha = a;
        }
        int flag   = -1; // 未标号：-1， 已标号，未检查：0， 已标号，已检查：1
        int parent = -1; // 父亲节点
        int alpha  = 0;  // 改进量
    };
    class Fordfulkson
    {
    public:
        explicit Fordfulkson(Graph* g);
        void     run(int source, int target);
        
    private:
        int         source;       // 源点
        int         target;       // 汇点
        int         maxFlow;      // 网络最大流
        Graph*      graph;        // 有向图
        
        deque<int>  nodeQue;      // BFS的队列
        vector<Labeltype> labelVec; //标记

        vector<vector<int>> historyPath;  // 历史增广路径
    };

};

#endif