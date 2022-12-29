#pragma once
#include <iostream>
#include <vector>

using  std::vector;
using  std::cout;
using  std::endl;

class  Point
{
    int x; //< 物理坐标
    int y;
    
    bool isBlock; // 标记障碍物
    bool isVisit; // 访问标记
        
    double gValue; // start Point  到 current Point的距离代价
    double hValue; // current Point到 target  Point的距离代价
    double fitness; // 当前距离的预估代价： f = g + h 代价函数
    
    Point* parent; // 保存父亲节点

public:
    bool operator==(const Point& other)
    {
        return (this->x == other.x) && (this->y == other.y);
    }
    
    bool operator<(const Point& other)
    {
        return this->fitness < other.fitness;
    }
    bool operator<=(const Point& other)
    {
        return this->fitness <= other.fitness;
    }
    bool operator>(const Point& other)
    {
        return this->fitness > other.fitness;
    }
    bool operator>=(const Point& other)
    {
        return this->fitness >= other.fitness;
    }

    Point(int a = 0, int b =0):x(a),y(b)
    {
        isBlock = false;
        isVisit = false;
        gValue = 0;
        hValue = 0;
        fitness = 0;
        parent = nullptr;
    }

    bool isVisit() const { return isVisit;};
    void setVisited()  {isVisit = true;} ;
    void unSetVisite() {isVisit = false;};
    
    bool isObstacle() const { return isBlock;};
    void setObstacle() {isBlock = true;};
    void unsetObstacle() {isBlock = false;};

    Point* getParent() const{return parent;};
    void   setParent(Point* p) { parent = p;};

    void setFitness(double g, double h){ fitness = g+h;};
    double getFitness() const {return fitness;};
};

class Graph
{
public:
    // 默认地图size(20,20)空白
    Graph(int w = 20, int h=20);
    // 导入一外部地图
    Graph(const vector<vector<int>> &originMap);
    

    // 将外部输入地图网格
    void constructGraph();

    // 8邻接, 4邻接
    vector<Point*> neighBours(const Point* curPoint, bool isFull);

    // 获取路径, 反向跟踪
    vector<Point*> calPath(const Point* resPoint);

    // 

private:
    int width;
    int height;
    vector<vector<Point*>> map;  // 存放构建的graph
    vector<Point*>         path; // 存放当前路径

    

};