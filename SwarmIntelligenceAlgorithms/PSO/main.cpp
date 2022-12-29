#include "pso.h"
using namespace std;
int main()
{
    long popNums = 10;
    double  w = 0.5;
    double c1 = 2;
    double c2 = 2;
    PSO psoAlgo= PSO(popNums, w, c1, c2);
    psoAlgo.run(100);
    return 0;
}