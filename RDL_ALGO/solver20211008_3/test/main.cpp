#include "RdlSolver.h"

//#include "nlohmann/json.hpp"
//using json = nlohmann::ordered_json;

int main()
{
    
    RdlSolver rdlSolver;
    //DbWrapper::DbWrapper *pDbWrapper = rdlSolver.getDbWrapper();
    //cout << pDbWrapper << endl;
    string strInputJson = "../design_data_cLoc.json";
    ifstream inFile(strInputJson);
    
    json myJson;
    inFile >> myJson;

    int OffsetDistance = 30;//global
    //parse floorplan_size
    int W_FP = myJson["FP_Size"][0];
    int H_FP = myJson["FP_Size"][1];
    
    rdlSolver.initGrid(0, 0, W_FP, H_FP, OffsetDistance);
    
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        DbWrapper::StruInstance struInstance;
        struInstance.strInstName = myJson["Chiplets"][i]["Name"];
        struInstance.nInstWidth = myJson["Chiplets"][i]["Size"][0];
        struInstance.nInstHeight = myJson["Chiplets"][i]["Size"][1];
        struInstance.nInstX = myJson["Chiplets"][i]["Location"][0];
        struInstance.nInstY = myJson["Chiplets"][i]["Location"][1];
        std::vector<std::string> vecPinNames;
        cout << "chiplet " << i << " Pin_Names " << myJson["Chiplets"][i]["Pin_Names"].size() << endl;
        for (size_t j = 0; j < myJson["Chiplets"][i]["Pin_Names"].size(); j++)
        {
            vecPinNames.push_back(myJson["Chiplets"][i]["Pin_Names"][j]);
        }
        std::vector<std::string> vecNetNames;
        for (size_t j = 0; j < myJson["Chiplets"][i]["Pin_Names"].size(); j++)
        {
            vecNetNames.push_back("");
        }
        std::vector<DbWrapper::StruPinLocation> vecPinLocations;
        cout << "chiplet " << i << " Pin_Locations " << myJson["Chiplets"][i]["Pin_Locations"].size() << endl;
        for (size_t j = 0; j < myJson["Chiplets"][i]["Pin_Locations"].size(); j++)
        {
            DbWrapper::StruPinLocation struPinLocation;
            struPinLocation.nPosX = myJson["Chiplets"][i]["Pin_Locations"][j][0];
            struPinLocation.nPosY = myJson["Chiplets"][i]["Pin_Locations"][j][1];
            vecPinLocations.push_back(struPinLocation);
        }
        rdlSolver.insertInstPins(struInstance, vecPinNames, vecNetNames, vecPinLocations);
    }
    
    int IO_pad_size = 50;//global
    int IO_pin_size = 10;//global

    int nMinSpace = myJson["Ext_Port_Min_Space"];
    int nPadWidth = IO_pad_size;
    int nPadHeight = IO_pad_size;
    int nPinWidth = IO_pin_size;
    int nPinHeight = IO_pin_size;
    std::vector<std::string> vecPinNames;
    std::vector<std::string> vecNetNames;
    for (size_t i = 0; i < myJson["Ext_Ports"].size(); i++)
    {
        vecPinNames.push_back(myJson["Ext_Ports"][i]);
        vecNetNames.push_back("");
    }
    std::vector<DbWrapper::StruPinLocation> vecIoPinLocations;
    for (size_t i = 0; i < myJson["Ext_Pin_Locations"].size(); i++)
    {
        DbWrapper::StruPinLocation struPinLocation;
        struPinLocation.nPosX = myJson["Ext_Pin_Locations"][i][0];
        struPinLocation.nPosY = myJson["Ext_Pin_Locations"][i][1];
        vecIoPinLocations.push_back(struPinLocation);
    }
    rdlSolver.initIoPins(nMinSpace, nPadWidth, nPadHeight, nPinWidth, nPinHeight, vecPinNames, vecNetNames, vecIoPinLocations);

    
    for (auto &x : myJson["Netlist"].items())
    {
        string strNetName = x.key();
        vector<string> vecConnectNames;
        for (size_t i = 0; i < x.value().size(); i++)
        {
            vecConnectNames.push_back(x.value()[i]);
        }
        rdlSolver.insertNet(strNetName, "", vecConnectNames);
    }
    vector<vector<Coord3>> routeData;//route path result
    vector<RouteVia> routeViaData; //route via resule
    rdlSolver.run(routeData, routeViaData);
    // cout << "route data " << routeData.size() << endl;
    // for (size_t i = 0; i < routeData.size(); i++)
    // {
    //     for (size_t j = 0; j < routeData[i].size(); j++)
    //     {
    //         cout << "( " << routeData[i][j].x << "," << routeData[i][j].y << "," << routeData[i][j].h << ") ";
    //     }
    //     cout << endl;
    // }
    

    // cout << "route via data " << routeViaData.size() << endl;
    // for (size_t i = 0; i < routeViaData.size(); i++)
    // {
    //     cout << "via " << routeViaData[i].x << "," << routeViaData[i].y << "," << routeViaData[i].h1 << "," << routeViaData[i].h2 << endl;
    // }
    return 0;
}