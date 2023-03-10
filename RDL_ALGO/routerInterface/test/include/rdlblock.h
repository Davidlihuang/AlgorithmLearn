#ifndef RDLBLOCK_H
#define RDLBLOCK_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Coord3;
class Tile;
class MazeBlock;
class Path;
class Chiplet;

class RDLBlock
{
public:
    Coord3 *coord3;
    Tile *parent_tile;
    //MazeBlock *parent_mb;
    int i, j, h;
    //bool is_terminal;
    //bool visited;
    bool is_occupied;
    string netname;
    Path *parent_path;

    RDLBlock(Coord3 *coord, int wire_spacing, int idx_i, int idx_j, int idx_h, Tile *init_tile);
    //RDLBlock *explore(vector<vector<vector<RDLBlock *>>> &empty_rdlblocks);
    void flip_corner(RDLBlock *(&rdlb_mid), RDLBlock *rdbl_lead, vector<vector<vector<RDLBlock *>>> vec_RDLBlock, Path *path);
    bool union_corners(RDLBlock *(&rdlb_mid), RDLBlock *rdbl_lead, vector<vector<vector<RDLBlock *>>> vec_RDLBlock);

    bool is_corner(RDLBlock *prev_rdlb, RDLBlock *next_rdlb);
    string get_corner_type(RDLBlock *prev_rdlb, RDLBlock *next_rdlb);
    void free_block();
    void occupy_block(Path *path);
};

class Path
{
public:
    vector<RDLBlock *> rdlb_path;
    string netname;
    bool is_IO;
    bool is_straight;
    int num_corners;
    bool is_multiPin;
    string pinname_multipin;
    vector<Coord3 *> coord3_path_uncleaned;
    vector<Coord3 *> coord3_path_cleaned;
    int WL;
    int via_count;

    Path(string Netname) : netname(Netname), is_IO(false), is_straight(false), WL(0), via_count(0) {}
    void move_RDLB(RDLBlock *from, RDLBlock *to);
    bool trace_path(int front_idx);

    int cal_corners();

    int get_idx_path(RDLBlock *rdlb);

    //available only for 2-pin net
    int get_idx_chiplet_for_first_rdl_in_path(vector<Chiplet *> vec_chiplet);
    int get_idx_chiplet_for_last_rdl_in_path(vector<Chiplet *> vec_chiplet);
};

vector<vector<vector<RDLBlock *>>> gen_empty_RDLBlock_vec(int max_i, int max_j, int max_h, int wire_spacing, vector<vector<Tile *>> vec_tile, int W_OB, int H_OB);
vector<Path *> parse_RDLBlock_vec(vector<vector<vector<RDLBlock *>>> &vec_RDLBlock, string filename_2_pin_nets_res,
                                  string filename_multi_pin_nets_res, int IOPad_size_and_offdist, int wire_spacing);
vector<RDLBlock *> get_middle_plus_des_RDLBs(RDLBlock *from, RDLBlock *to, vector<vector<vector<RDLBlock *>>> vec_RDLBlock);

//naive approach
void decrease_corners(vector<Path *> vec_path, vector<vector<vector<RDLBlock *>>> vec_RDLBlock);
int cal_cost_all_path(vector<Path *> vec_path);
vector<vector<Coord3 *>> transfer_paths_to_vec_coord3(vector<Path *> vec_path);
string get_corner_type(Coord3 *back, Coord3 *mid, Coord3 *front);

//union two corners into one
int corner_reduction_union(vector<Path *> vec_path, vector<vector<vector<RDLBlock *>>> vec_RDLBlock);

void gen_vec_coord3_uncleaned_for_paths(vector<Path *> vec_path);

#endif