#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include "nlohmann/json.hpp"
using json = nlohmann::ordered_json;

#define MAX_NUM 100000000

using namespace std;

class Tile;
class IO;
class BP;
class Edge;
class Node;
class MazeBlock;
class Coord3;
class Path;

class Coord
{
public:
    int x, y;
    Coord(int new_x, int new_y) : x(new_x), y(new_y) {}
    Coord(Coord *coord1) : x(coord1->x), y(coord1->y) {}
    void add(Coord *coord2)
    {
        this->x = this->x + coord2->x;
        this->y = this->y + coord2->y;
    }
    bool is_equal(Coord *c2)
    {
        if (this->x == c2->x && this->y == c2->y)
            return true;
        else
            return false;
    }
    void print_self()
    {
        cout << "(" << this->x << ", " << this->y << ") ";
    }

    void copy(Coord *coord2)
    {
        this->x = coord2->x;
        this->y = coord2->y;
    }

    int cal_dist(Coord *coord2)
    {
        return abs(this->x - coord2->x) + abs(this->y - coord2->y);
    }

    vector<Coord *> gen_vec_coord_between_two_coords(Coord *coord2_dest)
    {
        vector<Coord *> vec_coord;
        if (this->x == coord2_dest->x)
        {
            int dist = coord2_dest->y - this->y;
            if (dist > 0)
                for (int i = 1; i <= abs(dist); i++)
                {
                    Coord *new_coord = new Coord(this->x, this->y + i);
                    vec_coord.push_back(new_coord);
                }
            else if (dist < 0)
                for (int i = 1; i <= abs(dist); i++)
                {
                    Coord *new_coord = new Coord(this->x, this->y - i);
                    vec_coord.push_back(new_coord);
                }
            return vec_coord;
        }
        else if (this->y == coord2_dest->y)
        {
            int dist = coord2_dest->x - this->x;
            if (dist > 0)
                for (int i = 1; i <= abs(dist); i++)
                {
                    Coord *new_coord = new Coord(this->x + i, this->y);
                    vec_coord.push_back(new_coord);
                }
            else if (dist < 0)
                for (int i = 1; i <= abs(dist); i++)
                {
                    Coord *new_coord = new Coord(this->x - i, this->y);
                    vec_coord.push_back(new_coord);
                }
            return vec_coord;
        }
        else
            cout << "error: these two coords don't have the same x or y." << endl;
        return vec_coord;
    }
};

class Chiplet
{

public:
    int x, y;
    int width, height;
    int max_pin_row_num, max_pin_col_num;
    string name;
    vector<Coord *> unplaced_pinLocs;
    vector<Coord *> placed_pinLocs; //exclude used pinLocs
    vector<Coord *> used_pinLocs;
    vector<Coord *> placed_pinLocs_n_used_pinLocs;

    // for maze routing  3-pin nets
    vector<Coord *> placed_pinLocs_mr3;
    vector<Coord *> unplaced_pinLocs_mr3;
    vector<Coord *> placed_pinLocs_n_used_pinLocs_mr3;
    vector<Coord *> used_pinLocs_mr3;

    vector<Coord *> used_used_pinLocs_mr3;
    vector<Coord *> used_by_dummy_2pn_pinLocs_mr3;

    bool is_in_used_by_dummy_2pn_pinLocs_mr3(Coord *coord)
    {
        for (size_t i = 0; i < used_by_dummy_2pn_pinLocs_mr3.size(); i++)
        {
            if (coord->is_equal(used_by_dummy_2pn_pinLocs_mr3[i]))
                return true;
        }
        return false;
    }
    //

    //vector<vector<string>> signal_table;
    vector<string> signals;
    int num_s, num_t; //for two-pin nets
    //int pin_pitch;

    Chiplet(string new_name)
    {
        this->name = new_name;
        this->num_s = 0;
        this->num_t = 0;
    }

    bool is_in_chiplet(Tile *tile);
    bool is_coord_in_chiplet(Coord *coord);
    void gen_placed_pinLocs_n_used_pinLocs();
    void set_max_pin_row_num();
    void set_max_pin_col_num();
    void deep_cp_used_pinLocs_to_mr3()
    {
        for (size_t i = 0; i < used_pinLocs.size(); i++)
        {
            used_pinLocs_mr3.push_back(used_pinLocs[i]);
        }
    }
    Coord *get_n_erase_prior_coord();
};

class Pin
{
public:
    string pinname;
    int idx_chiplet;
    Pin(string new_pinname, int index_chiplet) : pinname(new_pinname), idx_chiplet(index_chiplet) {}
};

class Net
{
public:
    string netname;
    vector<string> pinnames;
    vector<Pin *> pinnames_structure;
    double delay;
    vector<Coord *> coord_pins;

    //Path *assigned_path;

    //for maze route
    vector<Coord *> coord_pins_mr3;
    bool is_IO_connection;

    //signal assignment
    vector<Coord *> assigned_coordinate;

    Net(string new_netname) : netname(new_netname), is_IO_connection(false) {}
    vector<Tile *> vec_tile;
    vector<MazeBlock *> used_mbs;
    vector<vector<Coord3 *>> vec_routing_path_coord3_cleaned;

    int get_num_pins() { return coord_pins.size(); }
    void add_new_pinname(string new_pinname) { pinnames.push_back(new_pinname); }
    void add_new_pinname_structure(Pin *new_pin) { pinnames_structure.push_back(new_pin); }
    void add_pinLocs_to_net(Coord *new_coord) { coord_pins.push_back(new_coord); }
    void convert_coordIO_to_coordBp(vector<IO *> vec_IO);
    void add_to_used_mbs(MazeBlock *mb);
    void set_parent_net_of_IO(vector<IO *> vec_IO);
    //void assign_path(Path *path) { this->assigned_path = path; }

    void add_pinLocs_to_net_mr3(Coord *new_coord) { coord_pins_mr3.push_back(new_coord); }
};

class IO_parse
{
public:
    int pad_size;
    int pin_size;
    vector<string> IOnames;
    vector<Coord *> placed_pinLocs;

    vector<Coord *> used_pinLocs;

    vector<Coord *> available_bp;
    vector<Coord *> used_bp;
    void add_new_io(string new_io_name)
    {
        IOnames.push_back(new_io_name);
    }
    void print_IO_pinLocs();
    void gen_available_bp(vector<vector<Tile *>> vec_tile);
    void route_to_nearest_bp();
    void print_available_bp();
    void use_bp(Coord *bp_coord);
};

class dist_cal
{
public:
    double distance;
    int idx1, idx2; //idx in the placed_pinLocs
    Coord *c1, *c2;
    dist_cal(double new_dis, int idx_1, int idx_2, Coord *c_1, Coord *c_2) : distance(new_dis), idx1(idx_1), idx2(idx_2), c1(c_1), c2(c_2) {}
    Coord cal_mid_coord();
};

class HPWL_idx
{
public:
    vector<int> vec_chiplet_idx;
    int HPWL;

    HPWL_idx(int hpwl) : HPWL(hpwl) {}
    void set_vec_chiplet_idx(vector<int> chiplet_idx)
    {
        for (size_t i = 0; i < chiplet_idx.size(); i++)
        {
            vec_chiplet_idx.push_back(chiplet_idx[i]);
        }
    }
};

// int cal_offset_boundary_width(int width_FL, int offset_distance, int IO_pad_size);
// int cal_offset_boundary_height(int height_FL, int offset_distance, int IO_pad_size);
//sort net by descending delay

//global functions
int cal_offset_boundary_width(int width_FL, int offset_distance, int IO_pad_size);
int cal_offset_boundary_height(int height_FL, int offset_distance, int IO_pad_size);
void sort_net_by_descending_delay(vector<Net *> &vec_net);
void sort_net_by_descending_wirebudget(vector<Net *> &vec_net, double clock_period);
void sort_net_by_ascending_wirebudget(vector<Net *> &vec_net, double clock_period);

double cal_manhattan_dist(Coord *c1, Coord *c2);
//void find_closest_pinpair_twochipeltsonly(vector<Coord *> &vec1, vector<Coord *> &vec2, vector<Coord *> &vec1_usedpin, vector<Coord *> &vec2_usedpin);
double cal_HPWL(vector<Coord *> vec_coord);
void gen_used_pin(vector<Chiplet *> &vec_chiplet, vector<Net *> vec_net, IO_parse *IOs_parse);

void pin_loc_output(string filename_pinLocs, vector<Chiplet *> vec_chiplet, IO_parse *IOs_parse);
void convert_coordIO_to_coordBp_for_vec_nets(vector<Net *> vec_net, vector<IO *> vec_IO);
int get_demand_supply(vector<Chiplet *> vec_chiplet);
void output_csv_files(vector<Edge *> vec_edge, vector<Node *> vec_node, int num_src, int demand_supply);
void set_demand_supply(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet);
void output_routing_path(vector<vector<Coord3 *>> vec_coord3_uncleaned, vector<BP *> allocated_bp, string Filename, int W_FP, int H_FP, int pad_size);
void output_3_pin_nets(vector<Net *> vec_net, int wire_spacing, string filename_maze_route_output);
char get_closest_side(Coord3 *coord3, int W_FP, int H_FP);
Coord3 *get_extended_coord3_to_IOpad(Coord3 *coord_bp, char side, int dist_to_fp_boundary);

void write_path(vector<Path *> vec_path, vector<BP *> allocated_bp, int W_FP, int H_FP, int pad_size);
void divide_paths(vector<Path *> vec_path_original, vector<Path *> &vec_path_2pin, vector<Path *> &vec_path_multipin);
void sort_2pin_paths_by_ascending_WL(vector<Path *> &vec_path_2pin);
void sort_2pin_paths_by_descending_WL(vector<Path *> &vec_path_2pin);

void read_paths_into_nets(vector<Net *> vec_net, vector<Path *> vec_path_2pin, vector<Path *> vec_path_multipin, vector<IO *> vec_IO, vector<Chiplet *> vec_chiplet);
void output_solution_to_json(char *input_json_filename, string filename_json, vector<Net *> vec_net);
void output_solution_to_json_2(char *input_json_filename, vector<Net *> vec_net, vector<IO *> vec_IO);

string find_coord_n_signal_in_net(Coord *coord, vector<Net *> vec_net);
void output_signal_names_as_csv_table(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet);

//for 2-pin net only
bool is_idx_chiplet_pair_match_pin_pair(int idx_chiplet1, int idx_chiplet2, int idx_chiplet_pin1, int idx_chiplet_pin2);

//for 3-pin nets' maze route
void gen_used_pin_mr3(vector<Chiplet *> &vec_chiplet, vector<Net *> vec_net, IO_parse *IOs_parse);
void output_3_pin_nets_mr3(vector<Net *> vec_net, int wire_spacing, string filename_maze_route_output);
string find_coord_n_signal_in_net_mr3(Coord *coord, vector<Net *> vec_net);
void output_signal_names_as_csv_table_mr3(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet);

void create_nets_3pn_dummy_2pn(vector<Net *> vec_net_origin, vector<Net *> &nets_3pn_dummy_2pn, int max_dummy_2pn);

#endif