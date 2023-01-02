#include "astar.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>

bool Astar::run(const Tile& start, const Tile& target)
{
    // 合法性
    if(!graph.isElementExist(start) || !graph.isElementExist(target))
    {
        cout << "start or end not in graph!" <<endl;
        return false;
    }
    // 取出图中的节点
    Tile* startTile = graph.findElement(start);
    Tile* targetTile = graph.findElement(target);
    if(startTile == nullptr || targetTile == nullptr)
     {
        std::cout <<"get start and end tile from graph failed!" << std::endl;
         return false;
     }  

    // 主要算法
    openTable.push_back(startTile);
    Tile* res = nullptr;
    while(!openTable.empty())
    {
        Tile* bestTile = openTable.front()
        if(bestTile == targetTile)
        {
            res = bestTile;
            break;
        }

        //< 获取所有邻居
        vector<Tile*> neibour = graph.neighBours(bestTile, false);
        for(int i=0; i<neibour.size(); i++)
        {
            //< 
            Tile* curTile = neibour[i];

            //< 计算各点的代价
            nCoord = curTile->getCoord();
            sCoord = bestTile->getCoord();
            tCoord = target.getCoord();
            double g = abs(nCoord.x - sCoord.x) + abs(nCoord.y - sCoord.y);
            double h = abs(tCoord.x - nCoord.x) + abs(tCoord.y - nCoord.y);
                

            //< 当前点在close表中什么也不做
            if((closeTable.find(curTile) != closeTable.end()) && 
                (bestTile->isObstacle()))
            {
                continue;
            }

            //< 节点在open表中的处理
            if(openTable.find(curTile) == openTable.end())
            {
                //< tile不在open表中
                curTile->parent = bestTile;
                curTile->setFitness(g, h);
                openTable.push_back(curTile);
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
        openTable.pop_front();
        closeTable.insert(bestTile);
    }

}