#include "astar.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>

bool Astar::run(const Tile& start, const Tile& target, bool direct)
{
    // 合法性
    if(!graph->isElementExist(start) || !graph->isElementExist(target))
    {
        cout << "start or end not in graph!" <<endl;
        return false;
    }

    // 取出图中的节点
    Tile* startTile = graph->findElement(start);
    Tile* targetTile = graph->findElement(target);
    if(startTile == nullptr || targetTile == nullptr)
     {
        std::cout <<"get start and end tile from graph failed!" << std::endl;
         return false;
     }  

    // 清空表
    closeTable.clear();
    while(!openTable.empty())
    {
        openTable.pop();
    }
    
    // 主要算法
    openTable.emplace(startTile);
    Tile* res = nullptr;
    while(!openTable.empty())
    {
        Tile* bestTile = openTable.top();
        openTable.pop();
        if(bestTile == targetTile)
        {
            res = bestTile;
            break;
        }

        //< 获取所有邻居
        // std::cout << "open表的大小：" << openTable.size() << std::endl;
        // showOpenTable();
        vector<Tile*> neibour = graph->neighBours(bestTile, direct);
        for(int i = 0; i < neibour.size(); i++)
        {
            //< 
            Tile* curTile = neibour[i];

            //< 计算各点的代价
            Point nCoord = curTile->getCoord();
            Point sCoord = bestTile->getCoord();
            Point tCoord = target.getCoord();
            double g = abs(nCoord.x - sCoord.x) + abs(nCoord.y - sCoord.y);
            double h = abs(tCoord.x - nCoord.x) + abs(tCoord.y - nCoord.y);
                

            //< 当前点在close表中什么也不做
            if(closeTable.find(curTile) != closeTable.end())
            {
                continue;
            }

            //< 节点在open表中的处理
            if(openTable.find(curTile) == openTable.end())
            {
                //< tile不在open表中
                curTile->setParent(bestTile);
                curTile->setFitness(g, h);
                openTable.emplace(curTile);
            }
            else{
                // tile在open表中                
                if(g < curTile->getFitness())
                {
                    curTile->setFitness(g,h);
                    curTile->setParent(bestTile);
                }
            }
        }
        
        
        closeTable.insert(bestTile);
    }

    if(res!= nullptr)
    {
        graph->calPath(res);
        return true;
    }

    std::cout <<"no resolve!" << std::endl;
    return false;

}