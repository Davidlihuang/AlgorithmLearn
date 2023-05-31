#include "grid.h"
#include <iostream>
#include "mcf.h"
#include <fstream>

void Graph::loadDesign(const std::string& filename)
{

}
void Graph::mapNetListToGraph()
{
    auto nlist = this->netList;
    std::cout << "originListnums:" << this->netList.size()<< std::endl;
    for (size_t n = 0; n < nlist.size(); n++)
    {
        std::cout << "1" << std::endl;
        auto net = nlist[n];
        auto pin = net.pins;
        if (pin.size() > 2)
            break;

        std::cout << "2" << std::endl;

        // 寻找pin附近的grid node
        auto pinNodesfirst  = findPinNodes(pin[0]);
        auto pinNodesSecond = findPinNodes(pin[1]);

        // 初始化pinNodes
        Node* srcPinNode = new Node(this->nodes.size(), pin[0]);
        srcPinNode->inDirect = false;
        srcPinNode->idx  = this->nodes.size();
        nodes.push_back(srcPinNode);
        
        

        Node* dstPinNode =  new Node(this->nodes.size(), pin[1]);
        dstPinNode->inDirect = true;
        dstPinNode->idx  = this->nodes.size();
        nodes.push_back(dstPinNode);
        

        // 存储pin的需求
        netNodes pinnodes(srcPinNode->idx, dstPinNode->idx, 1);
        netListNodes.push_back(pinnodes);
        std::cout << "netList nums: " << netListNodes.size() << std::endl;

        // 构建pin到附近指定(row, col) grid node的边
        for(auto c: pinNodesfirst)
        {
            
            if(c->inDirect == true)
            {
                Edge* pin2graph = new Edge(srcPinNode, c);
                pin2graph->idx  = this->arcs.size();
                pin2graph->capacity = 1;
                pin2graph->cost = 1;
                this->arcs.push_back(pin2graph);

            }
        }

        for(auto c: pinNodesSecond)
        {
            if(c->inDirect == false)
            {
                Edge* graph2pin=new Edge(c, dstPinNode);
                graph2pin->idx  = this->arcs.size();
                graph2pin->capacity = 1;
                graph2pin->cost = 1;
                this->arcs.push_back(graph2pin);
            }
        }

        
    }
}

void Graph::genNetworkFlowGraph()
{
    int pitch = this->wireSpace + this->wireWidth;
    int col  = std::floor(this->width/pitch);
    int row  = std::floor(this->height/pitch);
    vector<Node*> nodeIn;
    vector<Node*> nodeOut;
    vector<Edge*> interEdge;

    //构建grid node节点，以及内部边
    int interCap  = 1;
    int interCost = 1;
    for(int i =0; i< row; i++)
    {
        for(int j =0; j< col; j++)
        {
            //构建内部节点
            int idx = i*col + j;
            Node* node_in = new Node(idx, Coord(i, j));
            node_in->inDirect = true;
            nodeIn.push_back(node_in);
            
            Node* node_out = new Node(idx, Coord(i,j));
            node_out->inDirect = false;
            nodeOut.push_back(node_out);

            Edge* inEdge = new Edge(node_in, node_out);
            inEdge->capacity = 1;
            inEdge->cost     = interCost;
            interEdge.push_back(inEdge);

            // 将边保存到库中
            node_in->idx = this->nodes.size();
            this->nodes.push_back(node_in);
            
            node_out->idx = this->nodes.size();
            this->nodes.push_back(node_out);
            
            inEdge->idx = this->arcs.size();
            this->arcs.push_back(inEdge);
            
        }
    }

    //构建水平边
    int bcost = 0;
    for(int i =0; i< row; i++)
    {
        for(int j =0; j< col-1; j++)
        {
            //构建内部节点
            int idx = i*col + j;
            int secondIdx = idx+1;

            Edge* edgeIn_h  = new Edge(nodeOut[secondIdx], nodeIn[idx]);
            edgeIn_h->capacity = 1;
            edgeIn_h->cost   = bcost;
            edgeIn_h->idx = this->arcs.size();
            this->arcs.push_back(edgeIn_h);
            

            Edge* edgeOut_h = new Edge(nodeOut[idx], nodeIn[secondIdx]);
            edgeOut_h->capacity = 1;
            edgeOut_h->cost = bcost;
            edgeOut_h->idx = this->arcs.size();
            this->arcs.push_back(edgeOut_h);
            
        }
    }

    //构建垂直边
    for(int i =0; i< row-1; i++)
    {
        for(int j =0; j< col; j++)
        {
            int idx = i*col + j;
            int secondIdx = idx + col;

            Edge* edgeIn_v  = new Edge(nodeOut[secondIdx], nodeIn[idx]);
            edgeIn_v->capacity = 1;
            edgeIn_v->cost   = bcost;
            edgeIn_v->idx = this->arcs.size();
            this->arcs.push_back(edgeIn_v);
            

            Edge* edgeOut_v = new Edge(nodeOut[idx], nodeIn[secondIdx]);
            edgeOut_v->capacity = 1;
            edgeOut_v->cost = bcost;
            edgeOut_v->idx = this->arcs.size();
            this->arcs.push_back(edgeOut_v);
            
        }
    }

    // 接入外部pin到网络
    mapNetListToGraph();
    
    std::cout << "单节点数： " << nodeIn.size() << std::endl;
    std::cout << "总节点数： " << this->nodes.size() << std::endl;
    std::cout << "内部边： " << interEdge.size() << std::endl;
    std::cout << "总边数： " << this->arcs.size() << std::endl;
}
vector<Node*> Graph::findPinNodes(const Coord& p1)
{
    vector<Node*> nodeVec;
    for(size_t i=0; i< this->nodes.size() ; i++)
    {
        auto node = this->nodes[i];
        if(node->locate == p1)
        {
            nodeVec.push_back(node);
        }
    }
    return nodeVec;
}
void Graph::exportGraphToTxt(const std::string& filename)
{
    std::ofstream of(filename);
    
    size_t arcNums = this->arcs.size();
    size_t nodeNums = this->nodes.size();

    of<< nodeNums <<"\n";
    for(size_t i=0; i<  nodeNums; i++)
    {
        of<< this->nodes[i]->idx <<" " <<this->nodes[i]->locate.x <<" "<< this->nodes[i]->locate.y <<"\n";
    }

    of<< arcNums <<"\n";
    for(size_t j=0; j< arcNums; j++)
    {
        auto arc = this->arcs[j];
        
        int idx = arc->idx;
        int src = arc->srcNode->idx;
        int dst = arc->dstNode->idx;
        int cap = arc->capacity;
        int wgh = arc->cost;

        of<< idx <<" "<< src<<" " << dst <<" " << cap<<" " << wgh <<"\n";
    }

    //输出需求
    of << netListNodes.size() <<"\n";
    for(size_t m=0; m < netListNodes.size(); m++)
    {
        auto nets = netListNodes[m];
        of << m << " " << nets.pin1 << " " << nets.pin2 << " " << nets.supply <<"\n";
    }
    
    of.close();
}

void Graph::run()
{
    std::string designFile("");
    std::string outGraphFile("test.network");
    
    std::cout << "load design: " << designFile << std::endl;
    loadDesign(designFile);

    std::cout << "generation commodity flow graph..." << std::endl;
    genNetworkFlowGraph();

    std::cout << "export graph to textFile: " << outGraphFile << std::endl;
    exportGraphToTxt(outGraphFile);



}


