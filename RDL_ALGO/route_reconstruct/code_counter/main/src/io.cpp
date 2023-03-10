
#include "parse.h"
#include "bp.h"
#include "io.h"
#include "tile.h"

IO::IO(int x_, int y_, int IO_pad_size, int IO_pin_size, int W_FP, int H_FP)
{
    this->x = x_;
    this->y = y_;
    this->coord = new Coord(this->x, this->y);
    this->pad_size = IO_pad_size;
    this->pin_size = IO_pin_size;
    this->parent_net = nullptr;

    if (this->x == pad_size / 2)
        this->side = 'L';
    else if (this->x == W_FP - pad_size / 2)
        this->side = 'R';
    else if (this->y == pad_size / 2)
        this->side = 'D';
    else if (this->y == H_FP - pad_size / 2)
        this->side = 'U';
    else
    {
        cout << "error: an IO pad doeesn't belong to any side" << endl;
    }
}

void IO::search_track_and_mark(vector<Track_IO *> &vec_track, Coord *extended_coord)
{
    // cout << "extended_coord:";
    // extended_coord->print_self();
    // cout << endl;
    for (size_t i = 0; i < vec_track.size(); i++)
    {
        Track_IO *temp_track = vec_track[i];
        Coord *temp_coord = temp_track->coord;

        if (extended_coord->is_equal(temp_coord) && !(temp_track->used))
        {
            temp_track->use_track();
            this->used_track.push_back(i);
            //cout << "used marked:" << this->used_track.back() << endl;
            break;
        }
    }
}

void IO::extend_to_track(vector<Track_IO *> &vec_track, int x_OB, int y_OB)
{
    char side = this->side;
    int extend_x_dist = (x_OB - this->pad_size) / 2 + this->pad_size / 2;
    int extend_y_dist = (y_OB - this->pad_size) / 2 + this->pad_size / 2;
    Coord *new_extended_coord;
    if (side == 'L')
    {
        Coord *adder = new Coord(extend_x_dist, 0);
        new_extended_coord = this->add_coord(adder);
    }
    else if (side == 'U')
    {
        Coord *adder = new Coord(0, -extend_y_dist);
        new_extended_coord = this->add_coord(adder);
    }
    else if (side == 'D')
    {
        Coord *adder = new Coord(0, extend_y_dist);
        new_extended_coord = this->add_coord(adder);
    }
    else if (side == 'R')
    {
        Coord *adder = new Coord(-extend_x_dist, 0);
        new_extended_coord = this->add_coord(adder);
    }
    else
    {
        new_extended_coord = new Coord(0, 0);
        cout << "error: no such a side char" << endl;
    }
    this->coord_on_track = new_extended_coord;
    search_track_and_mark(vec_track, new_extended_coord);
}

void route_to_bp(vector<IO *> vec_IO, vector<Track_IO *> &vec_track, IO_parse *IOs_parse, int x_OB, int y_OB, int W_FP, int H_FP, vector<BP *> &vec_BP)
{
    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        IO *temp_IO = vec_IO[i];
        temp_IO->extend_to_track(vec_track, x_OB, y_OB);
    }

    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        IO *temp_IO = vec_IO[i];
        temp_IO->search_feasible_bp(IOs_parse, vec_track, x_OB, y_OB, W_FP, H_FP, vec_BP);
    }
}

vector<IO *> gen_IOs(IO_parse *IO_parse, int W_FP, int H_FP)
{
    vector<IO *> vec_IO;
    for (size_t i = 0; i < IO_parse->used_pinLocs.size(); i++)
    {
        Coord *temp_coord = IO_parse->used_pinLocs[i];
        IO *new_IO = new IO(temp_coord->x, temp_coord->y, IO_parse->pad_size, IO_parse->pin_size, W_FP, H_FP);
        vec_IO.push_back(new_IO);
    }

    return vec_IO;
}

void print_IOs(vector<IO *> vec_IO)
{
    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        cout << "(" << vec_IO[i]->x << ", " << vec_IO[i]->y << ")" << vec_IO[i]->side << " ";
    }
}

Coord *IO::add_coord(Coord *coord2)
{
    Coord *new_coord = new Coord(this->x + coord2->x, this->y + coord2->y);
    return new_coord;
}

Coord *IO::get_first_track_coord(vector<Track_IO *> vec_track)
{
    int idx_track = this->used_track[0];
    Track_IO *temp_track = vec_track[idx_track];
    return temp_track->coord;
}

Coord *IO::get_track_coord_before_bp(vector<Track_IO *> vec_track)
{
    int max_dist = 0;
    Coord *coord_nearest_bp = nullptr;
    for (size_t i = 0; i < this->used_track.size(); i++)
    {
        Track_IO *temp_track = vec_track[used_track[i]];
        Coord *temp_coord_on_track = temp_track->coord;
        Coord *temp_coord_first_track = this->get_first_track_coord(vec_track);

        int dist = temp_coord_first_track->cal_dist(temp_coord_on_track);
        if (max_dist < dist)
        {
            max_dist = dist;
            coord_nearest_bp = temp_coord_on_track;
        }
    }
    return coord_nearest_bp;
}

vector<Track_IO *> gen_routing_track_for_io_pad(int x_OB, int y_OB, int W_OB, int H_OB, int W_FP, int H_FP, int pad_size)
{
    vector<Track_IO *> track;

    //Down-track
    int height_track_from_fp_boundary = (y_OB + pad_size) / 2;
    for (int i = x_OB; i <= x_OB + W_OB; i++)
    {
        Coord *new_coord = new Coord(i, height_track_from_fp_boundary);
        Track_IO *new_io_track = new Track_IO(new_coord);
        track.push_back(new_io_track);
    }
    //Up-track
    for (int i = x_OB; i <= x_OB + W_OB; i++)
    {
        Coord *new_coord = new Coord(i, H_FP - height_track_from_fp_boundary);
        Track_IO *new_io_track = new Track_IO(new_coord);
        track.push_back(new_io_track);
    }
    //Left-track
    int width_track_from_fp_boundary = (x_OB + pad_size) / 2;
    for (int i = y_OB; i <= y_OB + H_OB; i++)
    {
        Coord *new_coord = new Coord(width_track_from_fp_boundary, i);
        Track_IO *new_io_track = new Track_IO(new_coord);
        track.push_back(new_io_track);
    }
    //Right-track
    for (int i = y_OB; i <= y_OB + H_OB; i++)
    {
        Coord *new_coord = new Coord(W_FP - width_track_from_fp_boundary, i);
        Track_IO *new_io_track = new Track_IO(new_coord);
        track.push_back(new_io_track);
    }

    return track;
}

// void print_io_tracks(vector<Track_IO *> vec_track)
// {
//     for (size_t i = 0; i < vec_track.size(); i++)
//     {
//         cout << "(" << vec_track[i]->coord->x << ", " << vec_track[i]->coord->y << ") ";
//         if (i % 940 == 0 && i != 0)
//             cout << endl;
//     }
// }

bool IO::is_track_avaible_and_mark_as_used(Coord *src_coord_track, Coord *dest_coord_track, Coord *bp_coord, vector<Track_IO *> &vec_track, IO_parse *IOs_parse)
{
    vector<Track_IO *> vec_candidate_track;
    vector<int> vec_idx_candidate_track;
    vector<Coord *> vec_desired_coord = src_coord_track->gen_vec_coord_between_two_coords(dest_coord_track);
    for (size_t i = 0; i < vec_desired_coord.size(); i++)
    {
        Coord *temp_coord_desired = vec_desired_coord[i];
        for (size_t j = 0; j < vec_track.size(); j++)
        {
            Track_IO *temp_track = vec_track[j];
            Coord *temp_coord_track = temp_track->coord;

            if (temp_coord_desired->is_equal(temp_coord_track))
            {
                if (temp_track->used)
                    return false;
                else
                {
                    vec_candidate_track.push_back(temp_track);
                    vec_idx_candidate_track.push_back(j);
                    break;
                }
            }
        }
    }

    if (vec_desired_coord.size() != vec_candidate_track.size())
        cout << "error: not enough track_io for desired coords...something wrong" << endl;

    for (size_t i = 0; i < vec_candidate_track.size(); i++)
    {
        vec_candidate_track[i]->use_track();
    }

    for (size_t i = 0; i < vec_idx_candidate_track.size(); i++)
    {
        this->used_track.push_back(vec_idx_candidate_track[i]);
    }

    return true;
}

void IO::search_feasible_bp(IO_parse *IOs_parse, vector<Track_IO *> &vec_track, int x_OB, int y_OB, int W_FP, int H_FP, vector<BP *> &vec_BP)
{
    Coord *temp_coord_on_track = this->get_first_track_coord(vec_track);
    //int idx_track_starting_point = this->used_track[0];
    int x_track = temp_coord_on_track->x;
    int y_track = temp_coord_on_track->y;
    vector<pair<Coord *, int>> vec_candidate_bp_coord;
    //cout << this->side << endl;
    if (this->side == 'L')
    {
        //cout << "this->side == 'L'" << endl;
        //cout << "vec_available_bp.size() " << vec_available_bp.size() << endl;
        for (size_t i = 0; i < IOs_parse->available_bp.size(); i++)
        {
            Coord *temp_coord = IOs_parse->available_bp[i];
            vector<Coord *> candidate_bp;
            int vertical_dist_to_bp = abs(y_track - temp_coord->y); //for 'L'
            //cout << "L, dist:" << vertical_dist_to_bp << "temp_coord->x == x_OB? " << temp_coord->x << " vs. " << x_OB << endl;
            if (!(vertical_dist_to_bp >= 10) || !(temp_coord->x == x_OB)) //for 'L'
                continue;
            else
            {
                //cout << "qualified...dist: " << vertical_dist_to_bp << ".. " << temp_coord->x << " vs. " << x_OB << endl;
                pair<Coord *, int> new_pair = make_pair(temp_coord, vertical_dist_to_bp); //for 'L'
                vec_candidate_bp_coord.push_back(new_pair);
                //push qualified coord into vector a and sort by distance
                //use pair
            }
        }
        sort(vec_candidate_bp_coord.begin(), vec_candidate_bp_coord.end(), [](pair<Coord *, int> a, pair<Coord *, int> b)
             { return a.second < b.second; });
        bool flag = false;
        while (!flag)
        {
            Coord *dest_bp_coord = vec_candidate_bp_coord.front().first;
            Coord *dest_coord_on_track = new Coord(x_track, dest_bp_coord->y); //for 'L'
            flag = is_track_avaible_and_mark_as_used(temp_coord_on_track, dest_coord_on_track, dest_bp_coord, vec_track, IOs_parse);
            vec_candidate_bp_coord.erase(vec_candidate_bp_coord.begin());
            if (flag)
            {
                IOs_parse->use_bp(dest_bp_coord);
                BP *new_bp = new BP(dest_bp_coord, this);
                this->BPptr = new_bp;
                vec_BP.push_back(new_bp);
            }
            if (vec_candidate_bp_coord.size() == 0)
                cout << "error: cannot find avaible track for this IO, 'L'" << endl;
        }
    }
    else if (this->side == 'D')
    {
        for (size_t i = 0; i < IOs_parse->available_bp.size(); i++)
        {
            Coord *temp_coord = IOs_parse->available_bp[i];
            vector<Coord *> candidate_bp;
            int horizontal_dist_to_bp = abs(x_track - temp_coord->x); //for 'D'
            //cout << "L, dist:" << vertical_dist_to_bp << "temp_coord->x == x_OB? " << temp_coord->x << " vs. " << x_OB << endl;
            if (!(horizontal_dist_to_bp >= 10) || !(temp_coord->y == y_OB)) //for 'D'
                continue;
            else
            {
                //cout << "qualified...dist: " << vertical_dist_to_bp << ".. " << temp_coord->x << " vs. " << x_OB << endl;
                pair<Coord *, int> new_pair = make_pair(temp_coord, horizontal_dist_to_bp); //for 'D'
                vec_candidate_bp_coord.push_back(new_pair);
                //push qualified coord into vector a and sort by distance
                //use pair
            }
        }
        sort(vec_candidate_bp_coord.begin(), vec_candidate_bp_coord.end(), [](pair<Coord *, int> a, pair<Coord *, int> b)
             { return a.second < b.second; });
        bool flag = false;
        while (!flag)
        {
            Coord *dest_bp_coord = vec_candidate_bp_coord.front().first;
            Coord *dest_coord_on_track = new Coord(dest_bp_coord->x, y_track); //for 'D'
            flag = is_track_avaible_and_mark_as_used(temp_coord_on_track, dest_coord_on_track, dest_bp_coord, vec_track, IOs_parse);
            vec_candidate_bp_coord.erase(vec_candidate_bp_coord.begin());
            if (flag)
            {
                IOs_parse->use_bp(dest_bp_coord);
                BP *new_bp = new BP(dest_bp_coord, this);
                this->BPptr = new_bp;
                vec_BP.push_back(new_bp);
            }
            if (vec_candidate_bp_coord.size() == 0)
                cout << "error: cannot find avaible track for this IO, 'D'" << endl;
        }
    }
    else if (this->side == 'R')
    {
        for (size_t i = 0; i < IOs_parse->available_bp.size(); i++)
        {
            Coord *temp_coord = IOs_parse->available_bp[i];
            vector<Coord *> candidate_bp;
            int vertical_dist_to_bp = abs(y_track - temp_coord->y); //for 'R' and 'L'
            //cout << "L, dist:" << vertical_dist_to_bp << "temp_coord->x == x_OB? " << temp_coord->x << " vs. " << x_OB << endl;
            if (!(vertical_dist_to_bp >= 10) || !(temp_coord->x == W_FP - x_OB)) //for 'R'
                continue;
            else
            {
                //cout << "qualified...dist: " << vertical_dist_to_bp << ".. " << temp_coord->x << " vs. " << x_OB << endl;
                pair<Coord *, int> new_pair = make_pair(temp_coord, vertical_dist_to_bp); //for 'R' and 'L'
                vec_candidate_bp_coord.push_back(new_pair);
                //push qualified coord into vector a and sort by distance
                //use pair
            }
        }
        sort(vec_candidate_bp_coord.begin(), vec_candidate_bp_coord.end(), [](pair<Coord *, int> a, pair<Coord *, int> b)
             { return a.second < b.second; });
        bool flag = false;
        while (!flag)
        {
            Coord *dest_bp_coord = vec_candidate_bp_coord.front().first;
            Coord *dest_coord_on_track = new Coord(x_track, dest_bp_coord->y); //for 'R' and 'L'
            flag = is_track_avaible_and_mark_as_used(temp_coord_on_track, dest_coord_on_track, dest_bp_coord, vec_track, IOs_parse);
            vec_candidate_bp_coord.erase(vec_candidate_bp_coord.begin());
            if (flag)
            {
                IOs_parse->use_bp(dest_bp_coord);
                BP *new_bp = new BP(dest_bp_coord, this);
                this->BPptr = new_bp;
                vec_BP.push_back(new_bp);
            }
            if (vec_candidate_bp_coord.size() == 0)
                cout << "error: cannot find avaible track for this IO, 'R'" << endl;
        }
    }
    else if (this->side == 'U')
    {
        for (size_t i = 0; i < IOs_parse->available_bp.size(); i++)
        {
            Coord *temp_coord = IOs_parse->available_bp[i];
            vector<Coord *> candidate_bp;
            int horizontal_dist_to_bp = abs(x_track - temp_coord->x); //for 'U'&'D'
            //cout << "L, dist:" << vertical_dist_to_bp << "temp_coord->x == x_OB? " << temp_coord->x << " vs. " << x_OB << endl;
            if (!(horizontal_dist_to_bp >= 10) || !(temp_coord->y == H_FP - y_OB)) //for 'U'
                continue;
            else
            {
                //cout << "qualified...dist: " << vertical_dist_to_bp << ".. " << temp_coord->x << " vs. " << x_OB << endl;
                pair<Coord *, int> new_pair = make_pair(temp_coord, horizontal_dist_to_bp); //for 'U'&'D'
                vec_candidate_bp_coord.push_back(new_pair);
                //push qualified coord into vector a and sort by distance
                //use pair
            }
        }
        sort(vec_candidate_bp_coord.begin(), vec_candidate_bp_coord.end(), [](pair<Coord *, int> a, pair<Coord *, int> b)
             { return a.second < b.second; });
        bool flag = false;
        while (!flag)
        {
            Coord *dest_bp_coord = vec_candidate_bp_coord.front().first;
            Coord *dest_coord_on_track = new Coord(dest_bp_coord->x, y_track); //for 'U'&'D'
            flag = is_track_avaible_and_mark_as_used(temp_coord_on_track, dest_coord_on_track, dest_bp_coord, vec_track, IOs_parse);
            vec_candidate_bp_coord.erase(vec_candidate_bp_coord.begin());
            if (flag)
            {
                IOs_parse->use_bp(dest_bp_coord);
                BP *new_bp = new BP(dest_bp_coord, this);
                this->BPptr = new_bp;
                vec_BP.push_back(new_bp);
            }
            if (vec_candidate_bp_coord.size() == 0)
                cout << "error: cannot find avaible track for this IO, 'U'" << endl;
        }
    }
    else
    {
        cout << "error: wrong side char";
    }
}

void output_bp_routing(vector<IO *> vec_IO, vector<Track_IO *> vec_track)
{
    ofstream outfile("bp_routing.txt");
    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        IO *temp_io = vec_IO[i];
        Coord *temp_first_coord_on_track = temp_io->get_first_track_coord(vec_track);
        Coord *coord_bp = temp_io->BPptr->coord;
        outfile << "IO" << i << endl;
        outfile << temp_io->x << " " << temp_io->y << " - " << temp_first_coord_on_track->x << " " << temp_first_coord_on_track->y << endl;
        Coord *coord_on_track_near_bp = temp_io->get_track_coord_before_bp(vec_track);
        outfile << temp_first_coord_on_track->x << " " << temp_first_coord_on_track->y << " - " << coord_on_track_near_bp->x << " " << coord_on_track_near_bp->y << endl;
        outfile << coord_on_track_near_bp->x << " " << coord_on_track_near_bp->y << " - " << coord_bp->x << " " << coord_bp->y << endl;
    }
}

bool IO::is_Coord3_in_IO(Coord3 *coord3)
{
    double x = (double)coord3->x;
    double y = (double)coord3->y;

    double x_low = (double)this->x - (double)this->pad_size / 2.0;
    double y_low = (double)this->y - (double)this->pad_size / 2.0;

    double x_high = (double)this->x + (double)this->pad_size / 2.0;
    double y_high = (double)this->y + (double)this->pad_size / 2.0;

    if (x >= x_low && x <= x_high && y >= y_low && y <= y_high)
        return true;
    else
    {
        //cout << "(x, y) = (" << x << ", " << y << ") (x_low, y_low) = (" << x_low << ", " << y_low << ") (x_high, y_high) = (" << x_high << ", " << y_high << ")" << endl;
        return false;
    }
}