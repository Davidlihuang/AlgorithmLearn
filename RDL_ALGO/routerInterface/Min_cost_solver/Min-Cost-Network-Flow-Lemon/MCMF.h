#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <string>
#include <map>
#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>
#include <lemon/cost_scaling.h>

#include <chrono>

using namespace lemon;
using namespace std;
using namespace std::chrono;

using Weight = long;
using Capacity = long;
using Resource = long;

using Graph = SmartDigraph;

using GNode = Graph::Node;////
using Arc = Graph::Arc;

template <typename ValueType>
using ArcMap = SmartDigraph::ArcMap<ValueType>;

template <typename ValueType>
using NodeMap = SmartDigraph::NodeMap<ValueType>;

using NS = NetworkSimplex<SmartDigraph, Capacity, Weight>;
using CS = CostScaling<SmartDigraph, Capacity, Weight>;

class MCMF
{
private:
    void readNodeTable(int nodeID[100], long nodeSD[100]);
    void readArcTable(int arcFrom[158], int arcTo[158], int arcMin[158], long arcMax[158], long arcWeight[158]);
public:
    void run();
};

