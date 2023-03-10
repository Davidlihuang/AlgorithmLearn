#include "RdlSolver.h"

void RdlSolver::insertRoutingLayer(int nLayerIndex, const std::string &strLayerName, bool bPreferredDirection)
{
    m_dbWrapper.insertRoutingLayer(nLayerIndex, strLayerName, bPreferredDirection);
}

void RdlSolver::insertRoutingTrack(int nLayerIndex, int nInitTrack, int nNumTracks, int nTrackStep, bool bDirection)
{
    m_dbWrapper.insertRoutingTrack(nLayerIndex, nInitTrack, nNumTracks, nTrackStep, bDirection);
}

void RdlSolver::initGrid(long nMinX, long nMinY, long nMaxX, long nMaxY, int nOffsetDistance)
{
    m_dbWrapper.initGrid(nMinX, nMinY, nMaxX, nMaxY, nOffsetDistance);
}
void RdlSolver::initIoPins(int nMinSpace, int nPadWidth, int nPadHeight, int nPinWidth, int nPinHeight, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<DbWrapper::StruPinLocation> &vecIoPinLocations)
{
    m_dbWrapper.initIoPins(nMinSpace, nPadWidth, nPadHeight, nPinWidth, nPinHeight, vecPinNames, vecNetNames, vecIoPinLocations);
}

void RdlSolver::insertInstPins(const DbWrapper::StruInstance &struInstance, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<DbWrapper::StruPinLocation> &vecPinLocations)
{
    m_dbWrapper.insertInstPins(struInstance, vecPinNames, vecNetNames, vecPinLocations);
}

void RdlSolver::insertNet(const std::string &strNetName, const std::string &strSignalType, const std::vector<std::string> &vecConnectNames)
{
    m_dbWrapper.insertNet(strNetName, strSignalType, vecConnectNames);
}

void RdlSolver::insertBlockage(long nMinX, long nMinY, long nMaxX, long nMaxY, int nLayerIndex)
{
    m_dbWrapper.insertBlockage(nMinX, nMinY, nMaxX, nMaxY, nLayerIndex);
}


void RdlSolver::run(vector<vector<Coord3>> &routeData, vector<RouteVia> &routeViaData)
{
    {
        SdcParser SdcParser;
        string strSdcParser0 = "../input/part_main.sdc";
        string strSdcParser1 = "../input/part_mem1.sdc";
        string strSdcParser2 = "../input/part_mem2.sdc";
        string strOutPutFile = "../output/sdc_netlist_with_ascending_delay_wirebudget.out";
        SdcParser.run(strSdcParser0,strSdcParser2,strSdcParser2,strOutPutFile);
    }
    {
        Solver solver1;
        string strInputJson = "../input/design_data_cLoc.json";
        string strVc = "-vc";
        int nVc = 2;
        solver1.run(m_dbWrapper, routeData, strInputJson,strVc,nVc);
    }
    {
        MCMF mcmf;
        mcmf.run();
    }
    {
        Solver solver2;
        string strInputJson = "../input/design_data_cLoc.json";
        string strVc = "-vc";
        int nVc = 2;
        string strRs = "-rs";
        string strIt = "-it";
        int nIt = 10;
        solver2.run(m_dbWrapper, routeData, strInputJson,strVc,nVc,strRs,strIt,nIt);
    }
    //create via from routeData
    for (size_t i = 0; i < routeData.size(); i++)
    {
        if (routeData[i].size() >= 2)
        {
            for (size_t j = 1; j < routeData[i].size() - 1; j++)
            {
                RouteVia routeVia;
                if (routeData[i][j-1].h < routeData[i][j].h)
                {    
                    routeVia.x = routeData[i][j].x;
                    routeVia.y = routeData[i][j].y;
                    routeVia.h1 = routeData[i][j-1].h;
                    routeVia.h2 = routeData[i][j].h;
                    routeViaData.push_back(routeVia);
                }
                else if (routeData[i][j-1].h > routeData[i][j].h)
                {
                    routeVia.x = routeData[i][j].x;
                    routeVia.y = routeData[i][j].y;
                    routeVia.h1 = routeData[i][j].h;
                    routeVia.h2 = routeData[i][j-1].h;
                    routeViaData.push_back(routeVia);                    
                }
            }
        }
    }
}