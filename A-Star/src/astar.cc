#include "astar.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>
double Astar::calG(const Tile& start, const Tile& current)
{
    Point p1 = start.getCoord();
    Point p2 = current.getCoord();
    
    double g = (abs(p1.x - p2.x) + abs(p1.y - p2.y));
    if(abs(p1.x - p2.x) == 1 && abs(p1.y - p2.y)==1)
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
    Tile* targetTile = graph->findElement(target);

    startTile->setObstacle(start.isObstacle());
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
    startTile->setFitness(0, (abs(target.getCoord().x) + abs(target.getCoord().y)));
    
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
  
            //< 障碍或当前点在close表中，什么也不做
            if(curTile->isObstacle() || (closeTable.find(curTile) != closeTable.end()))
            {
                continue;
            }

            // tile在open表中  
            if(openTable.find(curTile) != openTable.end())
            {
                double g = calG(*startTile, *curTile);
                double h = calH(*curTile, *targetTile);      
                if(g < curTile->getFitness())
                {
                    curTile->setFitness(g,h);
                    curTile->setParent(bestTile);
                }
            }
            else //if(openTable.find(curTile) == openTable.end())
            {
                //< tile不在open表中
                double g = calG(*startTile, *curTile);
                double h = calH(*curTile, *targetTile);
                curTile->setFitness(g, h);
                curTile->setParent(bestTile);
                openTable.emplace(curTile);
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