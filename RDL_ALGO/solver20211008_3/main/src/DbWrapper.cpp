#include "DbWrapper.h"

#include <iostream>

namespace DbWrapper
{

void DbWrapper::insertRoutingLayer(int nLayerIndex, const std::string &strLayerName, bool bPreferredDirection)
{
        StruRoutingLayer struRoutingLayer;
        struRoutingLayer.bPreferredDirection = bPreferredDirection;
        if (strLayerName.length() == 0)
                struRoutingLayer.strLayerName = "metal" + std::to_string(nLayerIndex);
        else
                struRoutingLayer.strLayerName = strLayerName;
        m_routingLayers.insert(std::make_pair(nLayerIndex, struRoutingLayer));
}

void DbWrapper::insertRoutingTrack(int nLayerIndex, int nInitTrack, int nNumTracks, int nTrackStep, bool bDirection)
{
        StruRoutingTrack struRoutingLayer;
        struRoutingLayer.nLayerIndex = nLayerIndex;
        struRoutingLayer.nInitTrack = nInitTrack;
        struRoutingLayer.nNumTracks = nNumTracks;
        struRoutingLayer.nTrackStep = nTrackStep;
        struRoutingLayer.bDirection = bDirection;
        m_routingTracks.push_back(struRoutingLayer);
}

void DbWrapper::initGrid(long nMinX, long nMinY, long nMaxX, long nMaxY, int nOffsetDistance)
{
        m_grid.nMinX = nMinX;
        m_grid.nMinY = nMinY;
        m_grid.nMaxX = nMaxX;
        m_grid.nMaxY = nMaxY;
        m_grid.nOffsetDistance = nOffsetDistance;
}

void DbWrapper::initIoPins(int nMinSpace, int nPadWidth, int nPadHeight, int nPinWidth, int nPinHeight, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<DbWrapper::StruPinLocation> &vecIoPinLocations)
{
        m_ioPins.nMinSpace = nMinSpace;
        m_ioPins.nPadWidth = nPadWidth;
        m_ioPins.nPadHeight = nPadHeight;
        m_ioPins.nPinWidth = nPinWidth;
        m_ioPins.nPinHeight = nPinHeight;
        m_ioPins.vecPinNames = vecPinNames;
        m_ioPins.vecNetNames = vecNetNames;
        m_ioPins.vecPinLocations = vecIoPinLocations;
}

void DbWrapper::insertNet(const std::string &strNetName, const std::string &strSignalType, const std::vector<std::string> &vecConnectNames)
{
        StruNet struNet;
        struNet.strNetName = strNetName;
        struNet.strSignalType = strSignalType;
        struNet.vecConnectNames = vecConnectNames;
        m_nets.push_back(struNet);
}

void DbWrapper::insertInstPins(const StruInstance &struInstance, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<StruPinLocation> &vecPinLocations)
{
        StruInstPins struInstPins;
        struInstPins.struInstance = struInstance;
        struInstPins.vecPinNames = vecPinNames;
        struInstPins.vecNetNames = vecNetNames;
        struInstPins.vecPinLocations = vecPinLocations;
        m_instPins.push_back(struInstPins);
}

void DbWrapper::insertBlockage(long nMinX, long nMinY, long nMaxX, long nMaxY, int nLayerIndex)
{
        StruBlockage struBlockage;
        struBlockage.nMinX = nMinX;
        struBlockage.nMinY = nMinY;
        struBlockage.nMaxX = nMaxX;
        struBlockage.nMaxY = nMaxY;
        struBlockage.nLayerIndex = nLayerIndex;
        m_blockages.push_back(struBlockage);
}

}