#ifndef DBWRAPPER_h
#define DBWRAPPER_h

#include <string>
#include <vector>
#include <map>

namespace DbWrapper 
{

struct StruRoutingLayer
{
        StruRoutingLayer() : bPreferredDirection(false)
        {}
        std::string strLayerName;
        bool bPreferredDirection;
};

struct StruRoutingTrack
{
        StruRoutingTrack() : nLayerIndex(0), nInitTrack(0), nNumTracks(0), nTrackStep(0), bDirection(false)
        {}
        int nLayerIndex;
        int nInitTrack;
        int nNumTracks;
        int nTrackStep;
        bool bDirection;
};

struct StruInstance
{
        StruInstance() : nInstWidth(0), nInstHeight(0), nInstX(0), nInstY(0)
        {}
        std::string strInstName;
        int nInstWidth;
        int nInstHeight;
        int nInstX;
        int nInstY;
};

struct StruGrid
{
        StruGrid() : nMinX(0), nMinY(0), nMaxX(0), nMaxY(0), nOffsetDistance(0)
        {}
        long nMinX;
        long nMinY;
        long nMaxX;
        long nMaxY;
        int nOffsetDistance;
};

struct StruNet
{
        std::string strNetName;
        std::string strSignalType;
        std::vector<std::string> vecConnectNames;
};

struct StruPinLocation
{
        StruPinLocation() : nPosX(0), nPosY(0)
        {}
        int nPosX;
        int nPosY;
};

struct StruInstPins
{
        StruInstance struInstance;
        std::vector<std::string> vecPinNames;
        std::vector<std::string> vecNetNames;
        std::vector<StruPinLocation> vecPinLocations;
};

struct StruIoPins
{
        StruIoPins() : nMinSpace(0), nPadWidth(0), nPadHeight(0), nPinWidth(0), nPinHeight(0)
        {}
        int nMinSpace;
        int nPadWidth;
        int nPadHeight;
        int nPinWidth;
        int nPinHeight;
        std::vector<std::string> vecPinNames;
        std::vector<std::string> vecNetNames;
        std::vector<StruPinLocation> vecPinLocations;
};

struct StruBlockage
{
        StruBlockage() : nMinX(0), nMinY(0), nMaxX(0), nMaxY(0), nLayerIndex(0)
        {}
        long nMinX;
        long nMinY;
        long nMaxX;
        long nMaxY;
        int nLayerIndex;
};

class DbWrapper 
{
public:      
        void insertRoutingLayer(int nLayerIndex, const std::string &strLayerName, bool bPreferredDirection);
        void insertRoutingTrack(int nLayerIndex, int nInitTrack, int nNumTracks, int nTrackStep, bool bDirection);
        void initGrid(long nMinX, long nMinY, long nMaxX, long nMaxY, int nOffsetDistance);
        void initIoPins(int nMinSpace, int nPadWidth, int nPadHeight, int nPinWidth, int nPinHeight, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<StruPinLocation> &vecIoPinLocations);
        void insertInstPins(const StruInstance &struInstance, const std::vector<std::string> &vecPinNames, const std::vector<std::string> &vecNetNames, const std::vector<StruPinLocation> &vecPinLocations);
        void insertNet(const std::string &strNetName, const std::string &strSignalType, const std::vector<std::string> &vecConnectNames); //connectNames is Netlist's value in Json file
        void insertBlockage(long nMinX, long nMinY, long nMaxX, long nMaxY, int nLayerIndex);

        std::map<int, StruRoutingLayer> m_routingLayers;
        std::vector<StruRoutingTrack> m_routingTracks;
        StruGrid m_grid;
        std::vector<StruNet> m_nets;
        std::vector<StruInstPins> m_instPins;
        StruIoPins m_ioPins;
        std::vector<StruBlockage> m_blockages;

        const static bool HORIZONTAL = false;
        const static bool VERTICAL = true;
};

}

#endif
