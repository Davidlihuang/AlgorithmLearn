#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#define INF 10000000
#define MAXN  21

int n, m, Edge[MAXN][MAXN];

int lowcost[MAXN], nearvex[MAXN];

void prim(int u0)
{
    int sumWeight =0;
    // first vertex initial
    for(int i=0;  i< n; i++)
    {
        lowcost[i] = Edge[u0][i];
        nearvex[i] = u0;
    }

    nearvex[u0] = -1;

    for(int i=0; i< n-1; i++)
    {
        int min = INF, v = -1;
        for(int j = 0; j < n; j++)
        {
            if(nearvex[j] != -1  && lowcost[j]< min) // find the minium vertex v
            {
                v = j;
                min = lowcost[j]; 
            }
        }

        if(v != -1)
        {
            printf("%d - %d : %d\n", nearvex[v], v, lowcost[v]); //mst edge

            nearvex[v] = -1;
            sumWeight += lowcost[v];

            // update lowcost , and nearvex
            for(int j=0; j< n; j++)
            {
                if(nearvex[j] != -1 && Edge[v][j]< lowcost[j])
                {
                    lowcost[j] = Edge[v][j];
                    nearvex[j] =v;
                }
            }
        }

    }
    printf("weight of MST is %d\n", sumWeight);
}

int main()
{
    int i, j, u, v,w;
    // read graph
    while (scanf("%d%d", &n, &m) != EOF)
    {
        memset(Edge, 0, sizeof(Edge));
        for(int i =0; i< m; i++)
        {
            scanf("%d%d%d",&u, &v, &w);
            Edge[u][v] = Edge[v][u] = w;
        }

        for(int i=0; i< n; i++)
        {
            for(int j=0; j< n; j++)
            {
                if(i == j)
                     Edge[i][j] = 0;
                else if(Edge[i][j] == 0) 
                    Edge[i][j] = INF;
            }
        }
    // gen mst
        prim(0);
    }


   
    
    return 0;
}