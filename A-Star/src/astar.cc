#include "astar.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>
double Astar::calG(const Tile& centerTile, const Tile& neiborTile)
{
    Point p1 = centerTile.getCoord();
    Point p2 = neiborTile.getCoord();
    double g = 0;
    double parentG = 0;
    g = (centerTile.getParent()==nullptr? 0 :centerTile.getParent()->getGvalue())  + (abs(p1.x - p2.x) + abs(p1.y - p2.y));
    if(abs(p1.x - p2.x) == 1 && abs(p1.y - p2.y))
    {
        g = 0.5*g; // << 优先走斜边
    }
    return g;
}
double Astar::calH(const Tile& curTile, const Tile& targetTile)
{
    double h =0;
    Point p1 = curTile.getCoord();
    Point p2 = targetTile.getCoord();
    h = (abs(p1.x - p2.x) + abs(p1.y - p2.y));
    return h;
}
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
    startTile->setObstacle(start.isObstacle());
    Tile* targetTile = graph->findElement(target);
    targetTile->setObstacle(target.isObstacle());

    if(startTile == nullptr || targetTile == nullptr )
     {
        std::cout <<"get start and end tile from graph failed!" << std::endl;
        return false;
     }  
    // if(startTile->isObstacle() || targetTile->isObstacle())
    // {
    //     return false;
    // }
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
   
            //< 障碍或当前点在close表中，什么也不做
            if(curTile->isObstacle() || (closeTable.find(curTile) != closeTable.end()))
            {
                continue;
            }

            //< tile不在open表中
            if(openTable.find(curTile) == openTable.end())
            {
                double g = calG(*bestTile, *curTile);
                double h = calH(*curTile, *targetTile);
                curTile->setFitness(g, h);
                curTile->setParent(bestTile);
                openTable.emplace(curTile);
            }

            // tile在open表中  
            if(openTable.find(curTile) != openTable.end())
            {
                double g = calG(*bestTile, *curTile);    
                double h = calH(*curTile, *targetTile);         
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