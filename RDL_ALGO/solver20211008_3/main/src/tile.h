#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <stack>

using namespace std;

class Node;
class Coord;
class Chiplet;
class MazeBlock;
class Edge;
class BP;
class Net;

class Coord3
{
public:
    int x, y, h;
    Coord3(int _x, int _y, int _h) : x(_x), y(_y), h(_h) {}

    bool is_equal(Coord3 *othercoord)
    {
        if (this->x == othercoord->x && this->y == othercoord->y && this->h == othercoord->h)
            return true;
        else
            return false;
    }
    bool is_x_equal(Coord3 *othercoord)
    {
        if (this->x == othercoord->x && this->h == othercoord->h)
            return true;
        else
            return false;
    }
    bool is_y_equal(Coord3 *othercoord)
    {
        if (this->y == othercoord->y && this->h == othercoord->h)
            return true;
        else
            return false;
    }

    bool is_straitline(Coord3 *mid_coord, Coord3 *back_coord)
    {
        if (this->x == mid_coord->x && this->x == back_coord->x && this->h == mid_coord->h && this->h == back_coord->h)
            return true;
        else if (this->y == mid_coord->y && this->y == back_coord->y && this->h == mid_coord->h && this->h == back_coord->h)
            return true;
        else
            return false;
    }
    int get_dist(Coord3 *coord2)
    {
        return abs(this->x - coord2->x) + abs(this->y - coord2->y);
    }
    void self_print()
    {
        cout << "(" << this->x << ", " << this->y << ", " << this->h << ") ";
    }

    bool layerchange(Coord3 *coord2)
    {
        if (this->h != coord2->h)
            return true;
        else
            return false;
    }
};

class Tile
{
public:
    Coord *coord;
    int x, y;
    Coord *coord_mid;
    int x_mid, y_mid;
    int width, height;
    bool is_terminal;
    int idx_i, idx_j;
    bool is_bp;
    BP *bp;
    //string netname;
    Net *net;
    int idx_chiplet;
    int id_pin_node;

    Tile *U, *L, *D, *R; //up, left, down, right

    Edge *via_edge_1to2, *via_edge_2to3;
    Edge *edge_rdl1, *edge_rdl2, *edge_rdl3;
    Edge *edge_rdl1_out_to_u1_out, *edge_rdl1_out_to_d1_out, *edge_rdl1_out_to_l1_out, *edge_rdl1_out_to_r1_out;
    Edge *edge_rdl1_out_to_via1to2_in;
    Edge *edge_bp;

    Node *rdl1_in, *rdl2_in, *rdl3_in;
    Node *rdl1_out, *rdl2_out, *rdl3_out;
    Node *via_node_1to2_in, *via_node_1to2_out;
    Node *via_node_2to3_in, *via_node_2to3_out;

    Node *u1_in, *l1_in, *d1_in, *r1_in;
    Node *u2_in, *l2_in, *d2_in, *r2_in;
    Node *u3_in, *l3_in, *d3_in, *r3_in;

    Node *u1_out, *l1_out, *d1_out, *r1_out;
    Node *u2_out, *l2_out, *d2_out, *r2_out;
    Node *u3_out, *l3_out, *d3_out, *r3_out;

    Node *node_bp_in, *node_bp_out;

    Coord *Ucoord, *Lcoord, *Dcoord, *Rcoord;
    bool Ubd = false, Lbd = false, Dbd = false, Rbd = false; //indicate if they are on the boundary side

    char bp_side; //bp is on U,D,R,L

    Tile(int x_, int y_, int wire_spacing, vector<Chiplet *> vec_chiplet, int idx_i, int idx_j, vector<Node *> &vec_node, vector<Edge *> &vec_edge, int num_tiles_h, int num_tiles_v);
    void set_as_Ubd() { this->Ubd = true; }
    void set_as_Lbd() { this->Lbd = true; }
    void set_as_Dbd() { this->Dbd = true; }
    void set_as_Rbd() { this->Rbd = true; }
    MazeBlock *get_mapped_mb_RDL1(vector<vector<vector<MazeBlock *>>> vec_mazeblock)
    {
        return vec_mazeblock[1][idx_i + 1][idx_j + 1];
    }
    MazeBlock *get_mapped_mb_RDL3(vector<vector<vector<MazeBlock *>>> vec_mazeblock)
    {
        return vec_mazeblock[3][idx_i + 1][idx_j + 1];
    }
    //void print_coord_mid() { this->coord_mid->print_self(); }
    //void print_coord_mid() { this->coord->print_self(); }
    void set_coordULDR();
    void attach_to_bp();

    void create_internal_edges(vector<Edge *> &vec_edge, int via_cost);
    void create_external_edges(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int cost, int num_tiles_h, int um_tiles_v);

    int get_id_pin_node();

    int get_idx_h(Node *node);
    Coord3 *get_coord3(int h);
    string get_node_name(Node *node);
};

vector<vector<Tile *>> gen_tile_vector(int x_OB, int y_OB, int W_OB, int H_OB, int wire_spacing, vector<Chiplet *> vec_chiplet,
                                       vector<Node *> &vec_node, vector<Edge *> &vec_edge, int num_tiles_h, int num_tiles_v);
void print_tile_coord(vector<vector<Tile *>> vec_tiles);
bool is_pin(Coord *coord_mid, vector<Chiplet *> vec_chiplet);
void attach_tiles_to_bp(vector<vector<Tile *>> vec_tile, vector<Node *> &vec_node);
void create_bpNode_inTile(vector<vector<Tile *>> vec_tile, vector<Node *> &vec_node);
void create_internal_edges_for_tiles(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int via_cost);
void create_extenal_edges_for_tiles(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int cost, int num_tiles_h, int num_tiles_v);
void assign_chiplet_to_all_tiles(vector<vector<Tile *>> vec_tile, vector<Chiplet *> vec_chiplet);
void del_res(vector<MazeBlock *> used_mb, vector<vector<vector<MazeBlock *>>> vec_mbs);
void del_res_for_all_tiles_by_nets(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> vec_mbs, ofstream &outfile);
int get_num_2_pin_net(vector<Net *> vec_net);
void assign_s_t_to_all_2_pin_tiles(vector<vector<Tile *>> vec_tile, vector<Chiplet *> vec_chiplet, vector<Edge *> &vec_edge, Node *s, Node *t, int cost);

//file reader
vector<pair<int, int>> read_flow_solution();
vector<vector<int>> gen_nodeID_sets(vector<pair<int, int>> pairs_nodeID, vector<Node *> vec_node);
vector<vector<Coord3 *>> gen_coord3_sets(vector<vector<int>> vec_nodeID_set, vector<Node *> vec_node, vector<vector<Tile *>> vec_tile);
void block_pin_rdl1_edge(vector<vector<Tile *>> vec_tile);

#endif