#ifndef _BIDBidirectAstar_H_
#define _BIDBidirectAstar_H_

#include "graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <functional>
#include <deque>

using namespace std;
struct Tilecmp
{
    bool operator()(const Tile* left, const Tile* right) const
	{
        //< 实现priority_queue的小顶堆
		return left->getFitness() > right->getFitness();
	}
};
template<
       class T, 
       class container = vector<T>,
	   class cmp  = greater<T>
       >
class pQueue: public priority_queue<T, container, cmp>
{
public:
    auto begin() {
        return priority_queue<T, container, cmp>::c.begin();
    }
    auto end() {
        return priority_queue<T, container, cmp>::c.end();
    }
    auto find(T val) {
        return std::find(begin(), end(), val);
    }
};

class BidirectAstar
{
public:
    explicit BidirectAstar(Graph* g):graph(g){};
    bool run(const Tile& start, const Tile& end, bool direct=false);
    double calG(const Tile& curTile, const Tile& neiborTile);
    double calH(const Tile& curTile, const Tile& targetTile);
    void showOpenTable()
    {
        std::cout << "openTable container size = " << openTableFore.size() <<std::endl;
        // for(auto itr = openTable.begin(); itr != openTable.end(); itr++)
        // {
        //     std::cout << "{("<< (*itr)->getRow() << "," << (*itr)->getCol() <<")-fitness:" << (*itr)->getFitness() << "}, ";
        // }
        auto cp = openTableFore;
        while(!cp.empty())
        {
            auto t = cp.top();
            std::cout << "fitness: " << t->getFitness() << ", " ;
            cp.pop();
        }
        std::cout << endl;
    }
    void getPath();
private:
    BidirectAstar();
    BidirectAstar(const BidirectAstar&);
    BidirectAstar& operator=(const BidirectAstar&);
    BidirectAstar& operator=(const BidirectAstar&) const;
    
private:    
    Graph* graph;
    pQueue<Tile*, std::vector<Tile*>, Tilecmp> openTableFore; // 前向优先队列
    pQueue<Tile*, std::vector<Tile*>, Tilecmp> openTableBack; // 前向优先队列
    unordered_set<Tile*>  closeTableFore;
    unordered_set<Tile*>  closeTableBack;
    vector<Tile*> Path;
};

#endif