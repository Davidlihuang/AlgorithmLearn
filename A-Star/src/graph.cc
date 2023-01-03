#include "graph.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

/**
 * @brief 构建默认地图
 *
 */
Graph::Graph(int row, int col)
{
    graph.clear();
    for (int i = 0; i < row; i++)
    {
        vector<Tile *> vecTile;
        for (int j = 0; j < col; j++)
        {
            Tile *tile = new Tile(Point(i, j), i, j);
            vecTile.push_back(tile);
        }
        graph.push_back(vecTile);
    }
    width = row;
    height = col;
    cout << "new graph(row:" << graph.size()
         << " ,col: " << graph[0].size() << ")" << endl;
}

Graph::Graph(const vector<vector<int>> &origingraph)
{
    if (origingraph.empty())
        return;
    int row = origingraph.size();
    int col = origingraph[0].size();
    width = row;
    height = col;
    graph.clear();
    for (int i = 0; i < row; i++)
    {
        vector<Tile *> vecTile;
        for (int j = 0; j < col; j++)
        {
            Tile *tile = new Tile(Point(i, j), i, j);
            if (origingraph[i][j] == 1)
            {
                tile->setObstacle(); // 设置为block
                tile->unSetVisited();
                tile->unsetParent();
            }
            vecTile.push_back(tile);
        }
        graph.push_back(vecTile);
    }
    cout << "new graph(row:" << graph.size()
         << " ,col: " << graph[0].size() << ")" << endl;
}

// 8邻接, 4邻接
vector<Tile *> Graph::neighBours(const Tile *curTile, bool isFull) //< Tile获取
{
    vector<Tile *> neighbor;
    if ((curTile == nullptr))
        return neighbor;
    
    
    if((curTile->getRow()<0 ||curTile->getRow() >= width) || (curTile->getCol()< 0 || curTile->getCol()>=height))
        return neighbor;


    int num = (isFull == true) ? 8 : 4;
    //< 8 近邻
    if (isFull == true)
    {
        int x[num] = {-1, 0, 1, 0, -1, -1, 1, 1};
        int y[num] = {0, -1, 0, 1, -1, 1, 1, -1};
        for (int i = 0; i < num; i++)
        {
            int r = curTile->getRow() + x[i];
            int c = curTile->getCol() + y[i];
            if((r<0 ||r >= width) || (c< 0 || c>=height))
                continue;
            auto e = graph[r][c];
            if(e->isObstacle() == false)
                neighbor.push_back(e);
        }
    }
    else
    {
        int x[num] = {-1, 0, 1, 0};
        int y[num] = {0, -1, 0, 1};
        for (int i = 0; i < num; i++)
        {
            int r = curTile->getRow() + x[i];
            int c = curTile->getCol() + y[i];
            if((r<0 ||r >= width) || (c< 0 || c>=height))
                continue;
            auto e = graph[r][c];
            if(e->isObstacle() == false)
                neighbor.push_back(e);
        }
    }
    // std::cout << "neibour: [";
    // for (int i = 0; i < neighbor.size(); i++)
    // {
    //     std::cout << "( " << neighbor[i]->getRow() << ", " << neighbor[i]->getCol() << " ), ";
    //      //std::cout << "( " << neighbor[i] << ", " << neighbor[i] << " ), ";
    // }
    // std::cout << "]"<<endl;
    return neighbor;
}

vector<Tile *> Graph::neighBours(int r, int c, bool isFull) // 行列获取
{
    vector<Tile *> vecTile;
    if ((r < 0 || r >= width) ||
        (c < 0 || c >= height))
        return vecTile;
    Tile *tile = graph[r][c];
    if(tile->isObstacle()) return vecTile;
    vecTile = this->neighBours(tile, isFull);
    return vecTile;
}
vector<Tile *> Graph::neighBours(const Point &p, bool isFull) //< 物理坐标获取
{
    vector<Tile*> vecTile;
    Tile tile(p);
    vecTile  = this->neighBours(&tile, isFull);
    return vecTile;
}
// 判断Tile是否在图中
bool Graph::isElementExist(const Tile &p) const
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j< height; j++)
        {
            if (p == *graph[i][j])
                return true;
        }
    }
    return false;
}
Graph::~Graph()
{
    for(int i = 0; i< width; i++)
    {
        for(int j = 0; j <height; j++)
        {
            //delete graph[i][j];
            ;
        }
    }
    graph.clear();
}

bool Graph::isElementExist(int r, int c) const
{
    if ((r < 0 || r >= width) ||
        (c < 0 || c >= height))
        return false;
    return true;
}

bool Graph::isElementExist(const Point &p) const
{
    Tile tile(p);
    return this->isElementExist(tile);
}
Tile* Graph::findElement(const Tile& p)
{
    for(int i=0; i< width; i++)
    {
        for(int j=0; j<height; j++)
        {
            Tile* node = graph[i][j];
            if(*node == p)
            {
                return node;
            }
        }
    }
    return nullptr;
}

vector<Tile*> Graph::calPath(Tile* resTile)
{
    
    if(resTile == nullptr) return this->path;
    auto tmpTile = resTile;
    while(tmpTile != nullptr)
    {
        tmpTile->setPathed();
        tmpTile->setObstacle();
        this->path.push_back(tmpTile);
        tmpTile = tmpTile->getParent();
    }
    //drawGraph();
    return this->path;
}


void Graph::drawGraph() const
{
    cout << "graph: " << endl;
    cout <<"   ";
    for (int j = 0; j < height; j++)
    {
        cout << j << ", " ;
    }
    cout << endl;
    for (int i = 0; i < width; i++)
    {
        cout << i <<":[";
        for (int j = 0; j < height; j++)
        {
            if(graph[i][j]->isPathed())
            {
                cout << "*" << ", ";
            }
            else{
               cout << graph[i][j]->isObstacle() << ", ";
            }
            
        }
        cout << "]" << endl;
    }
}
