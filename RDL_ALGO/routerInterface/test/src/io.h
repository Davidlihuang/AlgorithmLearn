#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Coord;
class Coord3;
class IO_parse;
class Track_IO;
class BP;
class Net;

class IO
{
public:
    int x, y;
    int w, h;
    int pad_size, pin_size;
    char side; //L, U, R, D//indicate which side this IO locate in the floorplane
    BP *BPptr;
    Coord *coord_on_track;
    Coord *coord;
    vector<int> used_track;
    Net *parent_net;
    //string IOname;

    IO(int x_, int y_, int IO_pad_size, int IO_pin_size, int W_FL, int H_FL);
    void search_track_and_mark(vector<Track_IO *> &vec_track, Coord *coord);
    void extend_to_track(vector<Track_IO *> &vec_track, int x_OB, int y_OB);
    Coord *add_coord(Coord *coord2);
    Coord *get_first_track_coord(vector<Track_IO *> vec_track);
    Coord *get_track_coord_before_bp(vector<Track_IO *> vec_track);
    void search_feasible_bp(IO_parse *IOs_parse, vector<Track_IO *> &vec_track, int x_OB, int y_OB, int W_FP, int H_FP, vector<BP *> &vec_BP);
    bool is_track_avaible_and_mark_as_used(Coord *src_coord_track, Coord *dest_coord_track, Coord *bp_coord, vector<Track_IO *> &vec_track, IO_parse *IOs_parse);
    bool is_Coord3_in_IO(Coord3 *coord3);
};

class Track_IO
{
public:
    Coord *coord;
    bool used;
    Track_IO(Coord *new_coord) : coord(new_coord), used(false) {}
    void use_track() { this->used = true; }
};

vector<IO *> gen_IOs(IO_parse *IO_parse, int W_FP, int H_FP);
void print_IOs(vector<IO *> vec_IO);
vector<Track_IO *> gen_routing_track_for_io_pad(int x_OB, int y_OB, int W_OB, int H_OB, int W_FP, int H_FP, int pad_size);
void print_io_tracks(vector<Track_IO *> vec_track);
void route_to_bp(vector<IO *> vec_IO, vector<Track_IO *> &vec_track, IO_parse *IOs_parse, int x_OB, int y_OB, int W_FP, int H_FP, vector<BP *> &vec_BP);
bool is_track_avaible_and_mark_as_used(Coord *src_coord_track, Coord *dest_coord_track, Coord *bp_coord, vector<Track_IO *> &vec_track, IO_parse *IOs_parse);
void output_bp_routing(vector<IO *> vec_IO, vector<Track_IO *> vec_track);
#endif