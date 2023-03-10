#ifndef SOLVER_H
#define SOLVER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <map>
#include <chrono>

#include "parse.h"
#include "node.h"
#include "tile.h"
#include "edge.h"
#include "io.h"
#include "bp.h"
#include "maze.h"
#include "rdlblock.h"
#include "nlohmann/json.hpp"

#include "DbWrapper.h"

#define MAX_NUM_INT 9999999

using json = nlohmann::ordered_json;

using namespace std;

class Solver
{
private:
    vector<Chiplet *> chiplets;
    int W_FP, H_FP; //floorplane dimension
    int W_OB, H_OB; //offset boundary dimension
    int x_OB, y_OB;
    int Offset_Distance = 30; //10 *3
    int IO_pad_size = 50;     //50x50
    int IO_pin_size = 10;     //50x50
    int wire_spacing = 20;    //wire mid-line spacing
    int numRDL = 3;
    int cost = 1, via_cost = 10;
    int iter_compaction = -1;
    double clock_period = 2.5;

    IO_parse *IOs_parse = new IO_parse;
    vector<Net *> nets;

    vector<vector<Tile *>> tiles;
    vector<IO *> IOs;
    vector<Track_IO *> routing_track_io_pad;
    vector<BP *> allocated_bp;
    vector<vector<vector<MazeBlock *>>> maze_blocks;
    vector<vector<vector<MazeBlock *>>> maze_blocks_randomized;
    vector<Edge *> edges;
    vector<Node *> nodes;
    vector<Path *> paths;

    Node *source_node = new Node(nullptr, nodes);
    Node *target_node = new Node(nullptr, nodes);

    bool is_run_maze_route_for_3pins = false;
    bool is_output_csv = false;
    bool is_read_flow_solution = false;

    //used from pin selection of 3 pin nets and added with a few dummy two pin nets.
    vector<Net *> nets_3pn_dummy_2pn;
    int max_num_dummy_2pn = 5;

    void parsing_from_json(const DbWrapper::DbWrapper &dbWrapper, const string &strJsonFilename);
    //void parsing_from_json(const string &strJsonFilename);
public:
    void run(const DbWrapper::DbWrapper &dbWrapper, vector<vector<Coord3>> &routeData, const string &strInput, const string &strVc, int nVc, const string &strRs = "", const string &strIt = "", int nIt = 0);
    //void run(const string &strInput, const string &strVc, int nVc, const string &strRs = "", const string &strIt = "", int nIt = 0);
};

#endif