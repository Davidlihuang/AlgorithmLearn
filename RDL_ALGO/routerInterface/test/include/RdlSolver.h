#ifndef RDLSOLVER_H
#define RDLSOLVER_H

#include "SdcParser.h"
#include "solver.h"
#include "MCMF.h"
#include "DbWrapper.h"

struct RouteVia
{
    RouteVia() : x(0), y(0), h1(0), h2(0)
    {}
    int x;
    int y;
    int h1; //layer 1
    int h2; //layer 2
};

class RdlSolver
{
public:
    //DbWrapper::DbWrapper *getDbWrapper()
    //{
        //return &m_dbWrapper;
    //}
    void insertRoutingLayer(int nLayerIndex, const std::string &strLayerName, bool bPreferredDirection);
    void insertRoutingTrack(int nLayerIndex, int nInitTrack, int nNumTracks, int nTrackStep, bool bDirection);
    void initGrid(long nMinX, long nMinY, long nMaxX, long nMaxY, int nOffsetDistance);
    void initIoPins(int nMinSpace, int nPadWidth, int nPadHeight, int nPinWidth, int nPinHeight, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<DbWrapper::StruPinLocation> &vecIoPinLocations);
    void insertInstPins(const DbWrapper::StruInstance &struInstance, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<DbWrapper::StruPinLocation> &vecPinLocations);
    void insertNet(const std::string &strNetName, const std::string &strSignalType, const std::vector<std::string> &vecConnectNames); //connectNames is Netlist's value in Json file
    void insertBlockage(long nMinX, long nMinY, long nMaxX, long nMaxY, int nLayerIndex);
    
    void run(vector<vector<Coord3>> &routeData, vector<RouteVia> &routeViaData);
private:
    DbWrapper::DbWrapper m_dbWrapper;
    vector<vector<Coord3>> routeData;
};

#endif