
#include "fordfulkerson.h"

using namespace std;
using namespace networkflow;

Fordfulkson::Fordfulkson(Graph* g):
    graph(g)
{
    Labeltype label;
    for(int i=0; i< graph->getNodesNums();i++)
    {
        labelVec.push_back(label); // n个节点标记初始化
    }
    
}

void  Fordfulkson::run(int source, int target)
{
    this->source = source;
    this->target = target;
    while(true) //穷尽所有增广路
    {
        // 源节点初始化
        labelVec[source].set(0, source, inf);
        this->nodeQue.push_back(source);

        //广度优先获取增广路径,汇点没有被标记时，没有找到增广路
        while(!nodeQue.empty() && (labelVec[target].flag == -1))
        {
            int vNode = this->nodeQue.front();
            this->nodeQue.pop_front();

            ///< 获取定点v的“正向弧”和“反向弧”
            //正向弧处理
            auto positiveNode = graph->getPositiveEdge(vNode);
            cout << "current V: " << vNode << " parent: " << labelVec[vNode].parent << endl;
            for(int i=0; i< positiveNode.size(); i++)
            {
                ArcType* arc = positiveNode[i];
                if(arc== nullptr) continue;

                int neiborV = arc->target;
                if(labelVec[neiborV].parent == -1)// 顶点未标号
                {
                    auto diffFlow = arc->cap - arc->flow ;
                    if(diffFlow > 0) // 边容量未满
                    {
                        labelVec[neiborV].flag = 0; //标记为标记未检查
                        labelVec[neiborV].parent = vNode; //标记父亲节点
                        labelVec[neiborV].alpha  = min(labelVec[vNode].alpha,diffFlow); // 改进量
                        nodeQue.push_back(neiborV);   //标记后节点放入队列
                        cout << vNode << "-> " << neiborV << endl;
                    }
                }
            }
            
            //反向弧处理
            auto negetiveNode = graph->getNegtiveEdge(vNode);
            for(int i=0; i< positiveNode.size(); i++)
            {
                ArcType* arc = positiveNode[i];
                if(arc== nullptr) continue;

                int neiborV = arc->source;
                if(labelVec[neiborV].parent == -1)// 顶点未标号
                {
                    auto diffFlow = arc->flow ;
                    if(diffFlow > 0) // 边容量未满
                    {
                        labelVec[neiborV].flag = 0;        //标记为标记未检查
                        labelVec[neiborV].parent = -vNode; //标记父亲节点
                        labelVec[neiborV].alpha  = min(labelVec[vNode].alpha,diffFlow); // 改进量
                        nodeQue.push_back(neiborV);   //标记后节点放入队列
                    }
                }
            }
            ///< 定点v：已标记和已检查
            labelVec[vNode].flag = 1;
        }// end BFS while

        
        //当汇点没有标号(s->t不可达)，或汇点调整量为零(增广完成)，结速程序
        if((labelVec[target].flag == -1) || (labelVec[target].alpha == 0))
            break;
        
        //保存历史增广路径
        int current = target;
        vector<int> path;  // 历史增广路径
        while(current != labelVec[current].parent)
        {

            path.push_back(current);
            current = abs(labelVec[current].parent);
        }
        this->historyPath.push_back(path);
        
        //调整网络可行流
        //回溯源点
        current = target;
        while(current != labelVec[current].parent)
        {
            
            int  parent = labelVec[current].parent;
            // 前向弧调整
            ArcType* arc = nullptr;
            if(parent>  0)
            {
                bool valid = graph->getEdge(parent, current, &arc);
                if(valid && (arc != nullptr))
                {
                    arc->flow += labelVec[current].alpha;
                }
            }
            else
            {
                // 后向弧调整
                arc = nullptr;
                bool valid = graph->getEdge(current, parent, &arc);
                if(valid && (arc!= nullptr))
                {
                    arc->flow -= labelVec[current].alpha;
                }
            }
            current = abs(parent);
        }

        
    } //end of while(true)
}