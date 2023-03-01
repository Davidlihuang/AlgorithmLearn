#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

const int MAX_ITER = 1000;   // 最大迭代次数
const double W = 0.8;        // 惯性因子
const double C1 = 1.5;       // 自我学习因子
const double C2 = 1.5;       // 社会学习因子
const double EPSILON = 1e-6; // 停止条件

// 边的结构体
struct Edge
{
    int u, v, type;

    Edge(int uu=0, int vv=0, int tt=0) : u(uu), v(vv), type(tt) {}
};

// 粒子的结构体
struct Particle
{
    vector<Edge> position; // 粒子当前的位置
    vector<Edge> velocity; // 粒子当前的速度
    double fitness;        // 粒子的适应度值
    Particle() : fitness(0.0) {}
};

// 多端线网的结构体
struct MultiNet
{
    int n;                 // 点的个数
    int m;                 // 边的个数
    vector<Edge> edges;    // 边的集合
    vector<vector<int>> G; // 点之间的邻接关系
};

// 计算多端线网的最小直角斯坦纳树代价
double calcCost(const MultiNet &net, const vector<Edge> &tree)
{
    int n = net.n;
    int m = tree.size();
    vector<vector<int>> adj(n, vector<int>(n, -1));
    for (int i = 0; i < m; i++)
    {
        int u = tree[i].u;
        int v = tree[i].v;
        int type = tree[i].type;
        adj[u][v] = type;
        adj[v][u] = type;
    }
    double cost = 0.0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (adj[i][j] != -1)
            {
                int cnt = 0;
                for (int k = 0; k < n; k++)
                {
                    if (adj[i][k] != -1 && adj[k][j] != -1)
                    {
                        cnt++;
                    }
                }
                if (cnt > 0)
                {
                    cost += sqrt(cnt) * adj[i][j];
                }
            }
        }
    }
    return cost;
}

// 随机生成一个多端线网
MultiNet generateRandomMultiNet(int n, int m)
{
    MultiNet net;
    net.n = n;
    net.m = m;
    net.G.resize(n);
    for (int i = 0; i < m; i++)
    {
        int u = rand() % n;
        int v = rand() % n;
        while (u == v || find(net.G[u].begin(), net.G[u].end(), v) != net.G[u].end())
        {
            u = rand() % n;
            v = rand() % n;
        }
        int type = rand() % 2;
        net.edges.push_back(Edge(u, v, type));
        net.G[u].push_back(v);
        net.G[v].push_back(u);
    }
    for(int i = 0; i< net.G.size(); i++)
    {
        std::cout << "[ ";
        for(int j = 0; j< net.G.size(); j++)
        {
            std::cout << net.G[i][j] << ", " ;
        }
        std::cout << " ],\n";
    }
    return net;
}

// 初始化粒子群
vector<Particle> initSwarm(int swarmSize, const MultiNet &net)
{
    vector<Particle> swarm(swarmSize);
    for (int i = 0; i < swarmSize; i++)
    {
        swarm[i].position = net.edges;
        swarm[i].velocity.resize(net.m);
        for (int j = 0; j < net.m; j++)
        {
            int type = rand() % 2;
            swarm[i].velocity[j] = Edge(0, 0, type);
        }
        swarm[i].fitness = calcCost(net, swarm[i].position);
    }
    return swarm;
}

// 更新粒子位置和速度
void updateParticle(Particle &pbest, Particle &gbest, Particle &particle, const MultiNet &net)
{
    int m = net.m;
    for (int i = 0; i < m; i++)
    {
        double r1 = (double)rand() / RAND_MAX;
        double r2 = (double)rand() / RAND_MAX;
        particle.velocity[i].type = W * particle.velocity[i].type + C1 * r1 * (pbest.position[i].type - particle.position[i].type) + C2 * r2 * (gbest.position[i].type - particle.position[i].type);
        if (particle.velocity[i].type < 0)
        {
            particle.velocity[i].type = 0;
        }
        else if (particle.velocity[i].type > 1)
        {
            particle.velocity[i].type = 1;
        }
        if (rand() % 2 == 0)
        {
            particle.position[i].type = 0;
        }
        else
        {
            particle.position[i].type = 1;
        }
    }
    double newFitness = calcCost(net, particle.position);
    if (newFitness < particle.fitness)
    {
        particle.fitness = newFitness;
        particle.position = particle.position;
    }
}

// 更新全局最优解
void updateGlobalBest(Particle &gbest, const Particle &particle)
{
    if (particle.fitness < gbest.fitness)
    {
        gbest.fitness = particle.fitness;
        gbest.position = particle.position;
    }
}

// 离散粒子群算法
vector<Edge> DPSO(const MultiNet &net)
{
    int swarmSize = 50;
    vector<Particle> swarm = initSwarm(swarmSize, net);
    Particle gbest;
    gbest.fitness = 1e9;
    for (int i = 0; i < swarmSize; i++)
    {
        if (swarm[i].fitness < gbest.fitness)
        {
            gbest = swarm[i];
        }
    }
    for (int iter = 0; iter < MAX_ITER; iter++)
    {
        for (int i = 0; i < swarmSize; i++)
        {
            updateParticle(swarm[i], gbest, swarm[i], net);
            updateGlobalBest(gbest, swarm[i]);
        }
    }
    return gbest.position;
}

// 测试
int main()
{
    srand(time(nullptr));
    MultiNet net = generateRandomMultiNet(10, 30);
    vector<Edge> tree = DPSO(net);
    double cost = calcCost(net, tree);
    cout << "最小直角斯坦纳树代价：" << cost << endl;
    
    return 0;
}
