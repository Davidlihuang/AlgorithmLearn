#include "graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <functional>
#include <deque>
#include "lee.h"

using namespace std;

bool Lee::run(const Tile& start, const Tile& target, bool direction)
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

    //startTile->setObstacle(start.isObstacle());
    //targetTile->setObstacle(target.isObstacle());

    if(startTile == nullptr || targetTile == nullptr )
     {
        std::cout <<"get start and end tile from graph failed!" << std::endl;
        return false;
     }  

     //intialize queue
     que.clear();
     que.push_back(startTile);
    startTile->setVisited();
     Tile* res = nullptr;
     while(!que.empty())
     {
        auto element = que.front();
        que.pop_front();
        if(element == nullptr)
            return false;
        
        if(element == targetTile)
        {
            res = element;
            path = graph->calPath(res);
            graph->clearVisitedAll();
            std::cout << "find path successful!" << std::endl;
            return true;
        }

        //< get neightbour and push to queue
        auto neibor = graph->neighBours(element, direction);
        for(int i=0;i< neibor.size();  i++)
        {
            auto cur = neibor[i];
            if(cur == nullptr) continue;

            que.push_back(cur);
            cur->setVisited();  //avoid back search
            cur->setParent(element);
        }

     }
     std::cout << "find path failed!" << std::endl;
     return false;
}
