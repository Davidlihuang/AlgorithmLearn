#include "MCMF.h"

// int nodeNum = 74052;
// int arcNum = 106837;

// void readNodeTable(int nodeID[74052], long nodeSD[74052]);
// void readArcTable(int arcFrom[106837], int arcTo[106837], int arcMin[106837], long arcMax[106837], long arcWeight[106837]);

void MCMF::readNodeTable(int nodeID[100], long nodeSD[100])
{

    fstream nodeColumn1;
    fstream nodeColumn2;

    string temp;
    string nodeIDVals, nodeSDVals;

    nodeColumn1.open("../input/Demo_Data/nodeID.csv", ios::in);
    nodeColumn2.open("../input/Demo_Data/nodeSD.csv", ios::in);

    int rowIndex = 0;

    //---------- Serially Reading Data in Node Table -------------
    while (nodeColumn1 >> temp && nodeColumn2 >> temp)
    {

        getline(nodeColumn1, nodeIDVals, ',');
        nodeID[rowIndex] = stoi(nodeIDVals);

        getline(nodeColumn2, nodeSDVals, ',');
        nodeSD[rowIndex] = stol(nodeSDVals);

        rowIndex++;
    }

    // nodeID[0] = 1;
    // nodeID[1] = 2;
    //-------------------------------------------------------------
}

void MCMF::readArcTable(int arcFrom[158], int arcTo[158], int arcMin[158], long arcMax[158], long arcWeight[158])
{

    fstream arcColumn1;
    fstream arcColumn2;
    fstream arcColumn3;
    fstream arcColumn4;
    fstream arcColumn5;

    string temp;
    string fromIDVals, toIDVals, arcMinVals, arcMaxVals, arcWeightVals;

    arcColumn1.open("../input/Demo_Data/arcFrom.csv", ios::in);
    arcColumn2.open("../input/Demo_Data/arcTo.csv", ios::in);
    arcColumn3.open("../input/Demo_Data/arcMin.csv", ios::in);
    arcColumn4.open("../input/Demo_Data/arcMax.csv", ios::in);
    arcColumn5.open("../input/Demo_Data/arcWeight.csv", ios::in);

    int rowIndex = 0;

    //---------- Serially Reading Data from Arc Table -------------
    while (arcColumn1 >> temp && arcColumn2 >> temp && arcColumn3 >> temp && arcColumn4 >> temp && arcColumn5 >> temp)
    {

        getline(arcColumn1, fromIDVals, ',');
        arcFrom[rowIndex] = stoi(fromIDVals);

        getline(arcColumn2, toIDVals, ',');
        arcTo[rowIndex] = stoi(toIDVals);

        getline(arcColumn3, arcMinVals, ',');
        arcMin[rowIndex] = stoi(arcMinVals);

        getline(arcColumn4, arcMaxVals, ',');
        arcMax[rowIndex] = stol(arcMaxVals);

        getline(arcColumn5, arcWeightVals, ',');
        arcWeight[rowIndex] = stol(arcWeightVals);

        rowIndex++;
    }
    //--------------------------------------------------------------

    // arcMin[0] = 0;
    // arcMax[0] = 1000000L;
}
//template<size_t N>

//int main(int argc, char *argv[])
void MCMF::run()
{

    //------------------------------ Reading Data ---------------------------------------
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //----------- Read Node and Edge Number -----------------
    ifstream node_arc_Num("../input/Demo_Data/nodeNum_arcNum.csv");
    string str_nodeNum, str_arcNum, line;

    getline(node_arc_Num, line);
    getline(node_arc_Num, str_nodeNum, ',');
    getline(node_arc_Num, str_arcNum, ',');

    int nodeNum = stoi(str_nodeNum);
    int arcNum = stoi(str_arcNum);
    int nodeID[nodeNum];
    long nodeSD[nodeNum];

    //----------- Read Node Table -----------------
    readNodeTable(nodeID, nodeSD);

    //---------- Check Ingested Node Table --------
    // for(int i = 0; i < nodeNum; i++)
    // {
    //     cout << nodeID[i] << "   " << nodeSD[i] << endl;
    // }
    //----------------------------------------------

    //---------------------------------------------

    //----------- Read Arc Table -----------------

    int arcFrom[arcNum];
    int arcTo[arcNum];
    int arcMin[arcNum];
    long arcMax[arcNum];
    long arcWeight[arcNum];

    readArcTable(arcFrom, arcTo, arcMin, arcMax, arcWeight);

    // //---------- Check Ingested Arc Table --------
    // for(int i = 0; i < arcNum; i++)
    // {
    //     cout << arcFrom[i] << "   " << arcTo[i] << "   " << arcMin[i] << "   "  << arcMax[i] << "   "  << arcWeight[i] << endl;
    // }
    // //----------------------------------------------

    //---------------------------------------------

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //-----------------------------------------------------------------------------------

    //------------------------------ Consstructing Network Graph --------------------------
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Graph g;

    //------------- Constructing Nodes -------------

    GNode nodes[nodeNum];////
    NodeMap<Resource> supplyDemands(g);
    map<int, GNode> nodeIndexMap;////

    for (int i = 0; i < nodeNum; i++)
    {
        nodes[i] = g.addNode();
        nodeIndexMap[nodeID[i]] = nodes[i];
        supplyDemands[nodes[i]] = nodeSD[i];
    }
    //------------------------------------------

    //------------- Constructing Arcs/Edges -------------

    Arc arcs[arcNum];
    ArcMap<Weight> weights(g);
    ArcMap<Resource> lowerBounds(g);
    ArcMap<Capacity> capacities(g);

    for (int i = 0; i < arcNum; i++)
    {
        arcs[i] = g.addArc(nodeIndexMap[arcFrom[i]], nodeIndexMap[arcTo[i]]);
        weights[arcs[i]] = arcWeight[i];
        lowerBounds[arcs[i]] = arcMin[i];
        capacities[arcs[i]] = arcMax[i];
    }
    //------------------------------------------

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //-----------------------------------------------------------------------------------

    //-------------------------- Running Optimization Solver ----------------------------
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    NS solver(g);

    solver.lowerMap(lowerBounds).upperMap(capacities).costMap(weights).supplyMap(supplyDemands);

    ArcMap<Capacity> flows(g);

    NS::ProblemType status = solver.run();

    //string filename_output = "../../main/input/solution_nodeid.txt";////
    string filename_output = "../input/solution_nodeid.txt";
    ofstream outfile;
    outfile.open(filename_output);
    if (!outfile.is_open())
    {
        cout << "error: outfile " << filename_output << " cannot be opened." << endl;
        return;
    }

    outfile << "NodeIDSrc NodeIDTar" << endl;

    switch (status)
    {
    case NS::INFEASIBLE:
        cerr << "insufficient flow" << endl;
        break;

    case NS::OPTIMAL:
        solver.flowMap(flows);

        cout << "Successful Execution !" << endl;

        //cout << "\n------- Suggested Flows ---------" << endl;
        //cout << "\n";

        for (int i = 0; i < arcNum; i++)
        {
            if (flows[arcs[i]] != 0)
            {
                //cout << arcFrom[i] << " ----> " << arcTo[i] << "  : " << flows[arcs[i]] << "  ; Cost : " << flows[arcs[i]] * weights[arcs[i]] << endl;
                outfile << " " << arcFrom[i] << " " << arcTo[i] << endl;
            }
        }

        cout << "Total Cost = " << solver.totalCost() << endl;

        cout << "[file output] " << filename_output << endl;
        outfile.close();
        break;

    case NS::UNBOUNDED:
        cerr << "infinite flow" << endl;
        break;

    default:
        break;
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //-----------------------------------------------------------------------------------
}
