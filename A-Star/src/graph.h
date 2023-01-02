#pragma once
#include <iostream>
#include <vector>

using  std::vector;
using  std::cout;
using  std::endl;

struct Point
{
    double x;
    double y;
    
    Point(double a=0.0, double b=0.0):x(a),y(b){};
    bool operator==(Point& other)
    {
        return (this->x == other.x)&& (this->y==other.y);
    }
    bool operator==(const Point& other) const
    {
        return (this->x == other.x)&& (this->y==other.y);
    }
};
class  Tile
{

private:
    int row;     //< 图中的行列数
    int col;
    Point coord; //< 物理坐标

    bool isBlock; // 标记障碍物
    bool isVisit; // 访问标记
        
    double gValue; // start Tile  到 current Tile的距离代价
    double hValue; // current Tile到 target  Tile的距离代价
    double fitness; // 当前距离的预估代价： f = g + h 代价函数
    
    Tile* parent; // 保存父亲节点

public:
    bool operator==(Tile& other)
    {
        return (this->coord == other.coord);
    }
    bool operator==(const Tile& other) const
    {
        return (this->coord == other.coord);
    }
    
    bool operator<(const Tile& other)
    {
        return this->fitness < other.fitness;
    }
    bool operator<=(const Tile& other)
    {
        return this->fitness <= other.fitness;
    }
    bool operator>(const Tile& other)
    {
        return this->fitness > other.fitness;
    }
    bool operator>=(const Tile& other)
    {
        return this->fitness >= other.fitness;
    }

    Tile(const Point& p, int r = 0, int c =0):coord(p), row(r),col(c)
    {
        isBlock = false;
        isVisit = false;
        gValue = 0;
        hValue = 0;
        fitness = 0;
        parent = nullptr;

    }

    bool   isVisited() const { return isVisit;};
    void   setVisited()  {isVisit = true;} ;
    void   unSetVisited() {isVisit = false;};
    
    bool   isObstacle() const { return isBlock;};
    void   setObstacle() {isBlock = true;};
    void   unsetObstacle() {isBlock = false;};

    Tile*  getParent() const{return parent;};
    void   setParent(Tile* p) { parent = p;};
    void   unsetParent(){parent=nullptr;};

    void   setFitness(double g, double h){ fitness = g+h;};
    double getFitness() const {return fitness;};

    void setCoord(double a, double b) {coord = Point(a,b);};
    void setCoord(const Point& p){coord = p;};
    Point getCoord(){return coord;};

    void setRowCol(int r, int c){
        row = r;
        col = c;
    }
    std::pair<int, int> getRowCol()
    {
        return std::pair<int,int>(row,col);
    }
    int getRow() const{return row;};
    void setRow(int r) {row = r;};

    int getCol() const{return col;};
    void setCol(int c) {col = c;};

};

/**
 * @brief 基于网格的二维图
 * 
 */
class Graph
{
public:
    // 默认地图size(20,20)空白
    Graph(int row = 20, int col=20);
    // 导入一外部地图
    Graph(const vector<vector<int>> &origingraph);
    
    ~Graph();

    // 8邻接, 4邻接
    vector<Tile*> neighBours(const Tile* curTile, bool isFull); //< Tile获取
    vector<Tile*> neighBours(int r, int c, bool isFull); //行列获取
    vector<Tile*> neighBours(const Point& p, bool isFull);   //< 物理坐标获取

    // 获取路径, 反向跟踪
    vector<Tile*> calPath(const Tile* resTile);

    // 判断Tile是否在图中
    bool isElementExist(const Tile& p)  const;
    bool isElementExist(int r, int c)   const;
    bool isElementExist(const Point& p) const;
    
    Tile* findElement(const Tile& p);
    // 绘制地图到终端
    void drawGraph() const;

private:
    int width;
    int height;
    vector<vector<Tile*>> graph;  // 存放构建的graph
    vector<Tile*>         path; // 存放当前路径  

};