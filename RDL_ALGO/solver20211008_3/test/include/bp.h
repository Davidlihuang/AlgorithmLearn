#ifndef BP_H
#define BP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class IO;
class Tile;
class IO_parse;
class Coord;

class BP
{
public:
    Coord *coord;
    Tile *parent_tile;
    IO *IOptr;
    BP(Coord *new_coord, IO *sourceIO) : coord(new_coord), IOptr(sourceIO) {}
    void find_parent_tile(vector<vector<Tile *>> vec_tile);
};

void find_parent_tile_for_vec_BP(vector<vector<Tile *>> vec_tile, vector<BP *> vec_BP);

#endif