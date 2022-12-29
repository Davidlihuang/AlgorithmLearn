#pragma onece
#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>

template<class T>
struct Particle
{
    T x;//位置
    T v;//速度
    T curFit;
    T pBest;   //个体最优
    T pBestX;  //个体最优位置
    void print()
    {
        std::cout << "x: " << x
                   <<" v: " << v
                   << " curFit: " << curFit
                   << " pBest: " << pBest << std::endl;
    }
};

typedef Particle<double> dbParticle;
class PSO{
private:
    double  w;       // 速度惯性
    double  c1;      // 个体律
    double  c2;      // 社会学习律
    double  vMax;    // 最大速度
    long    popNums; // 种群大小
    long    iterMax; // 迭代数量
    dbParticle* particles; // 粒子种群

    double  gBest;   // 种群最优
    double  gBestX;  // 种群最优的位置

    double  solverMin; //解空间范围
    double  solverMax;
private:
    

    //更新目标函数
    void fitnessCalFunc()
    {
        for(int i =0; i< popNums; i++)
        {
            auto temp = particles;
            temp->curFit  = -1*temp->x*(temp->x -2); //目标函数
            ++temp; 
        }
    }
    
    double getMax(double a, double b)
    {
        return a>b?a:b;
    }
    
    //初始化pso
    void init()
    {
        solverMin = 0;
        solverMax = 2;
        particles = new dbParticle[popNums];
        // 每个粒子随机分布x, v
        static std::default_random_engine e(time(0));
        static std::uniform_real_distribution<double> pos(solverMin,solverMax);
        static std::uniform_real_distribution<double> ver(0,vMax);
        for(int i=0; i< popNums; i++)
        {
            auto temp = particles;
            temp->x      = pos(e); //随机
            temp->v      = ver(e);
            std::cout << "算法开始: (" << particles->x << ", " << particles->v << " )" << std::endl;

            temp->curFit = 0;
            temp->pBest  = 0;
            // temp->print();
            ++temp;
        }

        // 计算每个粒子当前适应度
        fitnessCalFunc();
        

        // 初始化个体最优位置，种群最优位置
        for(int i=0; i< popNums; i++)
        {
            auto temp = particles;
            temp->pBest = temp->curFit;
            temp->pBestX = temp->x;
            if(temp->pBest> gBest)
                gBest  = temp->pBest;
                gBestX = temp->x;
            ++temp;
        }
        
    }

public:
    PSO():popNums(10), w(0.5), c1(2), c2(0.5),particles(nullptr)
    {
        vMax = 0.1;
        gBest = 0;
    }
    PSO(long Nums, double w1, double c11, double c21):
        popNums(Nums),
        w(w1), c1(c11), c2(c21),
        particles(nullptr)
    {
        vMax = 0.1;
        gBest = 0;
        init();
    }
    ~PSO()
    {
        delete []particles;
    }

    void run(int max = 0)
    {
        if(max != 0)
        {
            iterMax = max;
        }
        static std::default_random_engine e(time(0));
        static std::uniform_real_distribution<double> ran(0,1);
        std::cout << "---algo run ---" << std::endl;
        if(particles == nullptr) return;

        for(int i = 0; i< iterMax; i++)
        {
            // 计算各个粒子的位置和速度
            for(int j =0; j< popNums; j++)
            {
                auto temp = particles;
                // temp->print();
                double inertia =  w* temp->v;
                double r1 = ran(e);
                double r2 = ran(e);
                //std::cout << "r1= " << r1 << " r2= " << r2 << std::endl;
                double selfFit =  c1*r1*(temp->pBestX - temp->x);
                double popFit  =  c2*r2*(gBestX - temp->x);
                double newV = inertia + selfFit + popFit;
                std::cout << newV << std::endl;
                temp->v = newV;
                
                if(std::abs(newV) > vMax)
                {
                    if(newV > 0)
                    {
                        temp->v = vMax;
                    }
                    else
                    {
                        temp->v = -vMax;
                    }
                    std::cout << "cur v: " << temp->v << std::endl;
                }

                temp->x += temp->v;
                // if(temp->x < solverMin)
                //     temp->x = solverMin;
                
                // if(temp->x > solverMin)
                //     temp->x = solverMax;
                // temp->print();
                ++temp;
            }
            
            //计算各个粒子的适应度
            fitnessCalFunc(); 

            //更新个体和种群极值
            for(int k = 0; k < popNums; k++)
            {
                auto temp = particles;
                if(temp->pBest < temp->curFit)
                {
                    temp->pBest = temp->curFit;
                    temp->pBestX = temp->x;
                }
                // temp->pBest = getMax(temp->pBest, temp->curFit);

                if(gBest < temp->pBest)
                    gBest = temp->pBest;
                    gBestX = temp->x;   
                std::cout << "粒子n"<< k <<"=";
                temp->print();
                
                temp++;
            }

            std::cout << "第" << (i+1) << "次迭代，全局最优解gBest= " << gBest << std::endl;
                      
        }
    }
};

