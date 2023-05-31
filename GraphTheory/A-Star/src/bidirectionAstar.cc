#include "bidirectionAstar.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>
double BidirectAstar::calG(const Tile& start, const Tile& current)
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
double BidirectAstar::calH(const Tile& curTile, const Tile& targetTile)
{
    double h =0;
    Point p1 = curTile.getCoord();
    Point p2 = targetTile.getCoord();
    h = (abs(p1.x - p2.x) + abs(p1.y - p2.y));
    return h;
}
bool BidirectAstar::run(const Tile& start, const Tile& target, bool direct)
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
    
    closeTableFore.clear();
    closeTableBack.clear();
    while(!openTableFore.empty())
    {
        openTableFore.pop();
    }
    while (!openTableBack.empty())
    {
        openTableBack.pop();
    }
    
    
    // 主要算法    
    startTile->setFitness(0, 0);
    openTableFore.emplace(startTile);
    targetTile->setFitness(0,0);
    openTableBack.emplace(targetTile);
    
    Tile* meetTile = nullptr;
    while(!openTableFore.empty() && !openTableBack.empty())
    {
    ///< 前向搜索
        Tile* bestBeforeTile = openTableFore.top();
        openTableFore.pop();
        if(closeTableBack.find(bestBeforeTile) != closeTableBack.end())
        {
            meetTile = bestBeforeTile;
            std::cout << "findResult In before expand" << std::endl;
            break;
        }

        vector<Tile*> neibour = graph->neighBours(bestBeforeTile, direct);
        for(size_t i = 0; i < neibour.size(); i++)
        {
            //< 
            Tile* curTile = neibour[i];
  
            //< 障碍或当前点在close表中，什么也不做
            if(curTile->isObstacle() || (closeTableFore.find(curTile) != closeTableFore.end()))
            {
                continue;
            }

            // tile在open表中  
            double g = bestBeforeTile->getGvalue() + calG(*bestBeforeTile, *curTile);
            double h = calH(*curTile, *targetTile);
            if(openTableFore.find(curTile) != openTableFore.end())
            {   
                if(g < curTile->getFitness())
                {
                    // 优先队列中保存的是指针，指针所指向对象的值改变时是否触发排序？ 测试会的
                    curTile->setFitness(g, h);
                    curTile->setForwardParent(bestBeforeTile);
                }
            }
            else //if(openTable.find(curTile) == openTable.end())
            {
                //< tile不在open表中
                curTile->setFitness(g, h);
                curTile->setForwardParent(bestBeforeTile);
                openTableFore.emplace(curTile);
            }
        }
        closeTableFore.insert(bestBeforeTile);
        

    ///< 后向搜索
        Tile* bestBackTile = openTableBack.top();
        openTableBack.pop();
        if(closeTableFore.find(bestBackTile) != closeTableFore.end())
        {
            meetTile = bestBackTile;
            std::cout << "findResult In backward expand" << std::endl;
            break;
        }

        vector<Tile*> neibour_b = graph->neighBours(bestBackTile, direct);
        for(size_t i = 0; i < neibour_b.size(); i++)
        {
            //< 
            Tile* curNeiTile = neibour_b[i];
  
            //< 障碍或当前点在close表中，什么也不做
            if(curNeiTile->isObstacle() || (closeTableBack.find(curNeiTile) != closeTableBack.end()))
            {
                continue;
            }

            // tile在open表中  
            double g = bestBackTile->getGvalue() + calG(*bestBackTile, *curNeiTile);
            double h = calH(*curNeiTile, *targetTile);
            if(openTableBack.find(curNeiTile) != openTableBack.end())
            {   
                if(g < curNeiTile->getFitness())
                {
                    // 优先队列中保存的是指针，指针所指向对象的值改变时是否触发排序？ 测试会的
                    curNeiTile->setFitness(g, h);
                    curNeiTile->setBackwardParent(bestBackTile);
                }
            }
            else //if(openTable.find(curTile) == openTable.end())
            {
                //< tile不在open表中
                curNeiTile->setFitness(g, h);
                curNeiTile->setBackwardParent(bestBackTile);
                openTableBack.emplace(curNeiTile);
            }
        }
        closeTableBack.insert(bestBackTile);
    }

    if(meetTile!= nullptr)
    {
        graph->calPath(meetTile);
        auto forward  = meetTile->getForwardParent();
        auto backward = meetTile->getBackwardParent();
        std::cout << "meetPoint_f: " << forward->getRow() << ", " << forward->getCol()  <<std::endl;
        std::cout << "meetPoint_b: " <<  backward->getRow() << ", " << backward->getCol() << std::endl;
        return true;
    }

    std::cout <<"no resolve!" << std::endl;
    return false;

}