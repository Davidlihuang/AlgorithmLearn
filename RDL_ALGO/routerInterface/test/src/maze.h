#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <iomanip>
#include <deque>

using namespace std;

class Tile;
class Coord;
class IO;
class BP;
class Net;
//class IO;

class MazeBlock
{
public:
    bool blocked;
    bool is_terminal;
    int dist;
    Coord *coord;
    Coord *coord_mid;
    Tile *parent_tile;
    Net *net;
    int idx_h, idx_i, idx_j;
    MazeBlock *mb_prev; //log that mb who explored this mb

    bool visited; //for DFS

    void explore();
    void trace_back();

    MazeBlock(Tile *tile, int layerNum, int h, int i, int j);
    MazeBlock(MazeBlock *mb);

    void mark_blocked() { this->blocked = true; }
    void unblock() { this->blocked = false; }
    void increment_dist() { this->dist++; }
    bool is_dest_mb(MazeBlock *des_mb) { return (this == des_mb); }

    MazeBlock *find_R(vector<vector<vector<MazeBlock *>>> vec_maze_block);
    MazeBlock *find_L(vector<vector<vector<MazeBlock *>>> vec_maze_block); //
    MazeBlock *find_U(vector<vector<vector<MazeBlock *>>> vec_maze_block);
    MazeBlock *find_D(vector<vector<vector<MazeBlock *>>> vec_maze_block);
    MazeBlock *find_T(vector<vector<vector<MazeBlock *>>> vec_maze_block);
    MazeBlock *find_B(vector<vector<vector<MazeBlock *>>> vec_maze_block);

    vector<MazeBlock *> back_trace_by_prev_mb(vector<vector<vector<MazeBlock *>>> vec_maze_block);
    MazeBlock *get_mb_in_another_vec_mb(vector<vector<vector<MazeBlock *>>> other_vec_maze_block);
    void print_idxhij();
    MazeBlock *get_mb_downstairs(vector<vector<vector<MazeBlock *>>> vec_maze_block)
    {
        if (this->idx_h == 1 || this->idx_h == 2) //1 or 2 is ok
            return vec_maze_block[this->idx_h + 1][this->idx_i][this->idx_j];
        else
            return nullptr;
    }

    MazeBlock *get_mb_upstairs(vector<vector<vector<MazeBlock *>>> vec_maze_block)
    {
        if (this->idx_h == 2 || this->idx_h == 3) //3 or 2 is ok
            return vec_maze_block[this->idx_h - 1][this->idx_i][this->idx_j];
        else
            return nullptr;
    }
    bool is_neighbor(MazeBlock *mb2, int wire_spacing);
};

class Route
{
public:
    vector<MazeBlock *> used_blocks;
    vector<MazeBlock *> src; // 1MB originally, increases gradually if net has 2+ pins
    vector<MazeBlock *> des; // 1MB for 2-pin net. more than 1MB for 3+ pins net
    bool check_is_dest_pin(MazeBlock *last_explored_mb, Coord *coord_des_pin);
    void use_maze_block(MazeBlock *vec_maze_block_decided);
};

vector<vector<vector<MazeBlock *>>> gen_maze_blocks(vector<vector<Tile *>> vec_tile, int numLayer);
void print_blocked_mazeblocks(vector<vector<vector<MazeBlock *>>> vec_mazeblock);
vector<vector<vector<MazeBlock *>>> copy_maze_blocks(vector<vector<vector<MazeBlock *>>> vec_maze_block);
bool maze_route(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP);
vector<MazeBlock *> explore_1_to_1(MazeBlock *mb_src, MazeBlock *mb_des, vector<vector<vector<MazeBlock *>>> &clone_vec_maze_block);
void explore_many_to_1(vector<MazeBlock *> vec_mb_src, MazeBlock *mb_des);
queue<MazeBlock *> explore_many_to_many(deque<MazeBlock> *dq_src_mb, deque<MazeBlock> *dq_des_mb, vector<vector<vector<MazeBlock *>>> vec_maze_block);
void map_IO_to_mazeblock(vector<IO *> vec_IO, vector<vector<vector<MazeBlock *>>> &vec_maze_block);
bool randomized_maze_route(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP, float &routability);
bool maze_route_for_3plus_pins_minus1(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP);
bool maze_route_for_3plus_pins(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP);
vector<MazeBlock *> route_net(Net *net, vector<vector<vector<MazeBlock *>>> vec_maze_block, vector<BP *> vec_BP);

//maze routing for mr3
bool maze_route_for_3plus_pins_mr3(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP);
vector<MazeBlock *> route_net_mr3(Net *net, vector<vector<vector<MazeBlock *>>> vec_maze_block, vector<BP *> vec_BP);

#endif