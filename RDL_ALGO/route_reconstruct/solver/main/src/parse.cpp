#include "node.h"
#include "edge.h"
#include "tile.h"
#include "maze.h"
#include "io.h"
#include "bp.h"
#include "parse.h"
#include "rdlblock.h"

using namespace std;

int cal_offset_boundary_width(int width_FL, int offset_distance, int IO_pad_size)
{
    return (width_FL - 2 * (offset_distance + IO_pad_size));
}

int cal_offset_boundary_height(int height_FL, int offset_distance, int IO_pad_size)
{
    return (height_FL - 2 * (offset_distance + IO_pad_size));
}
//
void sort_net_by_descending_delay(vector<Net *> &vec_net)
{
    sort(vec_net.begin(), vec_net.end(), [](Net *const &lhs, Net *const &rhs)
         { return lhs->delay > rhs->delay; });

    //find those Net with delay -1
    vector<Net *> vec_net_with_minusOne_delay;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->delay < 0)
            vec_net_with_minusOne_delay.push_back(temp_net);
    }

    //move those Net with delay -1 to the front and give it a high priority
    for (size_t i = 0; i < vec_net_with_minusOne_delay.size(); i++)
    {
        Net *copy_temp_net = vec_net_with_minusOne_delay[i];
        for (size_t j = 0; j < vec_net.size(); j++)
        {
            Net *temp_net = vec_net[j];
            if (copy_temp_net == temp_net)
            {
                vec_net.erase(vec_net.begin() + j);
                vec_net.insert(vec_net.begin(), copy_temp_net);
            }
        }
    }
}
//
void sort_net_by_descending_wirebudget(vector<Net *> &vec_net, double clock_period)
{
    sort(vec_net.begin(), vec_net.end(), [](Net *const &lhs, Net *const &rhs)
         { return lhs->delay > rhs->delay; });

    //find those Net with wire_budget > 0
    vector<Net *> vec_net_with_highest_priority;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->delay > 0)
            vec_net_with_highest_priority.push_back(temp_net);
    }

    //move those Net with wire_budget > 0 to the front and give it the highest priority
    for (size_t i = 0; i < vec_net_with_highest_priority.size(); i++)
    {
        Net *copy_temp_net = vec_net_with_highest_priority[i];
        for (size_t j = 0; j < vec_net.size(); j++)
        {
            Net *temp_net = vec_net[j];
            if (copy_temp_net == temp_net)
            {
                vec_net.erase(vec_net.begin() + j);
                vec_net.insert(vec_net.begin(), copy_temp_net);
            }
        }
    }
}

//
void sort_net_by_ascending_wirebudget(vector<Net *> &vec_net, double clock_period)
{
    sort(vec_net.begin(), vec_net.end(), [](Net *const &lhs, Net *const &rhs)
         { return lhs->delay < rhs->delay; });
}
//
double cal_manhattan_dist(Coord *c1, Coord *c2)
{
    return abs(c1->x - c2->x) + abs(c1->y - c2->y);
}

void route_to_nearest_bp()
{
}

double cal_HPWL(vector<Coord *> vec_coord)
{
    int min_x = MAX_NUM, min_y = MAX_NUM, max_x = 0, max_y = 0;
    for (size_t i = 0; i < vec_coord.size(); i++)
    {
        if (vec_coord[i]->x < min_x)
            min_x = vec_coord[i]->x;
        if (vec_coord[i]->y < min_y)
            min_y = vec_coord[i]->y;
        if (vec_coord[i]->x > max_x)
            max_x = vec_coord[i]->x;
        if (vec_coord[i]->y > max_y)
            max_y = vec_coord[i]->y;
    }
    return abs(max_x - min_x) + abs(max_y - min_y);
}

// vector<dist_cal> permute(vector<dist_cal> &vec_candidate, vector<Chiplet *> vec_chiplet, vector<int> vec_idx_chiplet, int idx_vec_idx_chiplet)
// {
//     if (idx_vec_idx_chiplet == (int)(vec_idx_chiplet.size() - 1))
//         return vec_candidate;
//     int idx_chiplet = vec_idx_chiplet[idx_vec_idx_chiplet];

//     for (size_t i = 0; i < vec_chiplet[idx_chiplet]->placed_pinLocs.size(); i++)
//     {
//     }

//     idx_vec_idx_chiplet++;
//     return permute(vec_candidate, vec_chiplet, vec_idx_chiplet, idx_vec_idx_chiplet);
// }

// void generate_used_pins_greedy_method(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet)
// {
//     //nets should be sorted by delay time already
//     for (size_t i = 0; i < vec_net.size(); i++)
//     {
//         //use the chiplet idx vector extracted from netlist to assign used pins in chiplets
//         vector<int> idx_chip_in_net;
//         for (size_t j = 0; j < vec_net[i]->pinnames_structure.size(); j++)
//         {
//             Pin *temp_pin = vec_net[i]->pinnames_structure[j];
//             idx_chip_in_net.push_back(temp_pin->idx_chiplet);
//         }

//         //
//         //double min_dist = MAX_NUM;

//         if (vec_chiplet.size() == 2)
//         {
//             for (size_t j = 0; j < idx_chip_in_net.size(); j++)
//             {
//                 for (size_t k = 0; k < idx_chip_in_net.size(); k++)
//                 {
//                     if (j == k)
//                         continue;
//                     vector<Coord *> vec_coord1 = vec_chiplet[0]->placed_pinLocs;
//                     vector<Coord *> vec_coord2 = vec_chiplet[1]->used_pinLocs;
//                     vector<Coord *> vec1_usedpin = vec_chiplet[0]->placed_pinLocs;
//                     vector<Coord *> vec2_usedpin = vec_chiplet[1]->used_pinLocs;
//                     find_closest_pinpair_twochipeltsonly(vec_coord1, vec_coord2, vec1_usedpin, vec2_usedpin);
//                 }
//             }
//         }
//         //more than 2 chiplets...need recursive method
//         else
//         {
//             vector<dist_cal> vec_dist_cal_candidate;
//             for (size_t j = 0; j < idx_chip_in_net.size(); j++)
//             {
//                 for (size_t k = 0; k < idx_chip_in_net.size(); k++)
//                 {
//                     if (j == k)
//                         continue;
//                 }
//             }
//         }
//     }
// }

// vector<Coord *> copy_vector_ptrCoord(vector<Coord *> vec_coord1)
// {
//     vector<Coord *> vec_coord2;
//     for (size_t i = 0; i < vec_coord1.size(); i++)
//     {
//         Coord *new_coord = new Coord(vec_coord1[i]);
//         vec_coord2.push_back(new_coord);
//     }
//     return vec_coord2;
// }

void gen_used_pin(vector<Chiplet *> &vec_chiplet, vector<Net *> vec_net, IO_parse *IOs_parse)
{
    //cout << "gen_used_pin" << endl;
    // vector<Coord *> vec_coord1, vec_coord2, vec3, vecIO;
    // vec_coord1 = vec_chiplet[0]->placed_pinLocs;
    // vec_coord2 = vec_chiplet[1]->placed_pinLocs;
    // vec3 = vec_chiplet[2]->placed_pinLocs;
    // vecIO = IOs_parse->placed_pinLocs;

    //int count = 0;
    for (size_t i = 0; i < vec_net.size(); i++) ///< 处理所有nets的pin
    {
        //cout << count++ << endl;
        Net *temp_net = vec_net[i];
        vector<int> vec_idx_chiplet;
        //extract idx of chiplets need to be dealt
        for (size_t j = 0; j < temp_net->pinnames_structure.size(); j++) ///< 一个net中所有pin，涉及的chiplets
        {
            Pin *temp_pin = temp_net->pinnames_structure[j];
            vec_idx_chiplet.push_back(temp_pin->idx_chiplet);
        }

        //get used pins and distribute them to used_pin vector ///< 一个net只有两个chiplets
        if (vec_idx_chiplet.size() == 2)
        {
            //vec_coord1, vec_coord2
            vector<Coord *> *vec_coord1, *vec_coord2;

            //deep copy vec_coord1 and vec_coord2
            int idx1 = vec_idx_chiplet[0];
            int idx2 = vec_idx_chiplet[1];
            //deep copy vec_coord1
            
            //拿到net两个相连芯片的pin和其位置
            ///< 第一个chiplet的pin坐标
            if (idx1 == (int)(vec_chiplet.size()))
                vec_coord1 = &(IOs_parse->placed_pinLocs);//拿到外围IO的坐标   
            else
                vec_coord1 = &(vec_chiplet[idx1]->placed_pinLocs); //拿到chiplet的pin所有坐标

            //deep copy vec_coord2
            ///< 第二个chiplet的pin坐标
            if (idx2 == (int)(vec_chiplet.size()))     
                vec_coord2 = &(IOs_parse->placed_pinLocs);
            else
                vec_coord2 = &(vec_chiplet[idx2]->placed_pinLocs);

            //find min_HPWL pair in double for loop 最小周长pin pair
            double min_HPWL = MAX_NUM;
            int idx_j_min_HPWL = -1, idx_k_min_HPWL = -1;
            for (size_t j = 0; j < (*vec_coord1).size(); j++)
            {
                for (size_t k = 0; k < (*vec_coord2).size(); k++)
                {
                    Coord *coord1 = (*vec_coord1)[j];
                    Coord *coord2 = (*vec_coord2)[k];
                    vector<Coord *> vec_coord;
                    vec_coord.push_back(coord1);
                    vec_coord.push_back(coord2);
                    double HPWL = cal_HPWL(vec_coord);
                    //cout << HPWL << endl;
                    //cout << "try ";
                    if (HPWL < min_HPWL)
                    {
                        // cout << count++ << endl;
                        //cout << HPWL << endl;
                        min_HPWL = HPWL;
                        idx_j_min_HPWL = j;
                        idx_k_min_HPWL = k;
                    }
                    //cout << endl;
                }
            }

            //add selected pinLocs to the net， coord_pin// 有什么用？
            temp_net->add_pinLocs_to_net((*vec_coord1)[idx_j_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec_coord2)[idx_k_min_HPWL]);

            //place selected pin locs (Coord) to each chiplet/IOs_parse data structure respectively
            if (idx1 == (int)(vec_chiplet.size())) //is IO pin
            {
                IOs_parse->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
                (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);
            }
            else
            {
                vec_chiplet[idx1]->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
                (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);
            }

            if (idx2 == (int)(vec_chiplet.size())) //is IO pin
            {
                IOs_parse->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
                (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);
            }
            else
            {
                vec_chiplet[idx2]->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
                (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);
            }
        }
        else if (vec_idx_chiplet.size() == 3)
        {
            int idx1 = vec_idx_chiplet[0];
            int idx2 = vec_idx_chiplet[1];
            int idx3 = vec_idx_chiplet[2];
            vector<Coord *> *vec_coord1, *vec_coord2, *vec3;
            //deep copy vec_coord1 vec_coord2 vec3
            vec_coord1 = &(vec_chiplet[idx1]->placed_pinLocs);
            //cout << "vec_coord1 size: " << (*vec_coord1).size() << endl;
            vec_coord2 = &(vec_chiplet[idx2]->placed_pinLocs);
            vec3 = &(vec_chiplet[idx3]->placed_pinLocs);

            double min_HPWL = MAX_NUM;
            int idx_j_min_HPWL = -1, idx_k_min_HPWL = -1, idx_l_min_HPWL = -1;
            for (size_t j = 0; j < (*vec_coord1).size(); j++)
            {
                for (size_t k = 0; k < (*vec_coord2).size(); k++)
                {
                    for (size_t l = 0; l < (*vec3).size(); l++)
                    {
                        Coord *coord1 = (*vec_coord1)[j];
                        Coord *coord2 = (*vec_coord2)[k];
                        Coord *coord3 = (*vec3)[l];
                        vector<Coord *> vec_coord;
                        vec_coord.push_back(coord1);
                        vec_coord.push_back(coord2);
                        vec_coord.push_back(coord3);
                        double HPWL = cal_HPWL(vec_coord);
                        //cout << HPWL << endl;
                        if (HPWL < min_HPWL)
                        {
                            //cout << count++ << endl;
                            //cout << HPWL << endl;
                            min_HPWL = HPWL;
                            idx_j_min_HPWL = j;
                            idx_k_min_HPWL = k;
                            idx_l_min_HPWL = l;
                        }
                    }
                }
            }

            //add selected pinLocs to the net
            temp_net->add_pinLocs_to_net((*vec_coord1)[idx_j_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec_coord2)[idx_k_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec3)[idx_l_min_HPWL]);

            //place selected pin locs (Coord) to each chiplet's data structure respectively
            //remove 3 pins from the pool
            vec_chiplet[idx1]->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
            (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);

            vec_chiplet[idx2]->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
            (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);

            vec_chiplet[idx3]->used_pinLocs.push_back((*vec3)[idx_l_min_HPWL]);
            (*vec3).erase((*vec3).begin() + idx_l_min_HPWL);
        }
        else if (vec_idx_chiplet.size() == 4)
        {
            int idx1 = vec_idx_chiplet[0];
            int idx2 = vec_idx_chiplet[1];
            int idx3 = vec_idx_chiplet[2];
            int idx4 = vec_idx_chiplet[3];
            vector<Coord *> *vec_coord1, *vec_coord2, *vec3, *vec4;
            //deep copy vec_coord1 vec_coord2 vec3
            vec_coord1 = &(vec_chiplet[idx1]->placed_pinLocs);
            //cout << "vec_coord1 size: " << (*vec_coord1).size() << endl;
            vec_coord2 = &(vec_chiplet[idx2]->placed_pinLocs);
            vec3 = &(vec_chiplet[idx3]->placed_pinLocs);
            vec4 = &(vec_chiplet[idx4]->placed_pinLocs);

            double min_HPWL = MAX_NUM;
            int idx_j_min_HPWL = -1, idx_k_min_HPWL = -1, idx_l_min_HPWL = -1, idx_m_min_HPWL = -1;
            for (size_t j = 0; j < (*vec_coord1).size(); j++)
            {
                for (size_t k = 0; k < (*vec_coord2).size(); k++)
                {
                    for (size_t l = 0; l < (*vec3).size(); l++)
                    {
                        for (size_t m = 0; m < (*vec4).size(); m++)
                        {
                            Coord *coord1 = (*vec_coord1)[j];
                            Coord *coord2 = (*vec_coord2)[k];
                            Coord *coord3 = (*vec3)[l];
                            Coord *coord4 = (*vec4)[m];
                            vector<Coord *> vec_coord;
                            vec_coord.push_back(coord1);
                            vec_coord.push_back(coord2);
                            vec_coord.push_back(coord3);
                            vec_coord.push_back(coord4);
                            double HPWL = cal_HPWL(vec_coord);
                            //cout << HPWL << endl;
                            if (HPWL < min_HPWL)
                            {
                                //cout << count++ << endl;
                                //cout << HPWL << endl;
                                min_HPWL = HPWL;
                                idx_j_min_HPWL = j;
                                idx_k_min_HPWL = k;
                                idx_l_min_HPWL = l;
                                idx_m_min_HPWL = m;
                            }
                        }
                    }
                }
            }

            //add selected pinLocs to the net
            temp_net->add_pinLocs_to_net((*vec_coord1)[idx_j_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec_coord2)[idx_k_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec3)[idx_l_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec4)[idx_m_min_HPWL]);

            //place selected pin locs (Coord) to each chiplet's data structure respectively
            vec_chiplet[idx1]->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
            (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);

            vec_chiplet[idx2]->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
            (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);

            vec_chiplet[idx3]->used_pinLocs.push_back((*vec3)[idx_l_min_HPWL]);
            (*vec3).erase((*vec3).begin() + idx_l_min_HPWL);

            vec_chiplet[idx4]->used_pinLocs.push_back((*vec4)[idx_m_min_HPWL]);
            (*vec4).erase((*vec4).begin() + idx_m_min_HPWL);
        }

        else
        {

            cout << "error: wrong number of pins in a net" << endl;

            // vector<int> vec_idx;
            // vector<vector<Coord *> *> vec_vec_coord;
            // for (size_t j = 0; j < vec_idx_chiplet.size(); j++)
            // {
            //     vec_idx.push_back(vec_idx_chiplet[j]);
            // }

            // for (size_t j = 0; j < vec_idx.size(); j++)
            // {
            //     int temp_idx = vec_idx[j];
            //     vector<Coord *> *vec_coord = &(vec_chiplet[temp_idx]->placed_pinLocs);
            //     vec_vec_coord.push_back(vec_coord);
            // }

            // double min_HPWL = MAX_NUM;
        }

        //cout << "hello" << endl;
    }
    //cout << "hello final" << endl;
}

void pin_loc_output(string filename_pinLocs, vector<Chiplet *> vec_chiplet, IO_parse *IOs_parse)
{

    ofstream outfile(filename_pinLocs);
    //cout << "------chiplet 0 --------" << endl;
    for (size_t i = 0; i < vec_chiplet[0]->used_pinLocs.size(); i++)
    {
        //cout << chiplets[0]->used_pinLocs[i]->x << ", " << chiplets[0]->used_pinLocs[i]->y << endl;
        outfile << vec_chiplet[0]->used_pinLocs[i]->x << " " << vec_chiplet[0]->used_pinLocs[i]->y << endl;
    }
    //cout << "------chiplet 1--------" << endl;
    for (size_t i = 0; i < vec_chiplet[1]->used_pinLocs.size(); i++)
    {
        //cout << chiplets[1]->used_pinLocs[i]->x << ", " << chiplets[1]->used_pinLocs[i]->y << endl;
        outfile << vec_chiplet[1]->used_pinLocs[i]->x << " " << vec_chiplet[1]->used_pinLocs[i]->y << endl;
    }
    //cout << "------chiplet 2--------" << endl;
    for (size_t i = 0; i < vec_chiplet[2]->used_pinLocs.size(); i++)
    {
        //cout << chiplets[2]->used_pinLocs[i]->x << ", " << chiplets[2]->used_pinLocs[i]->y << endl;
        outfile << vec_chiplet[2]->used_pinLocs[i]->x << " " << vec_chiplet[2]->used_pinLocs[i]->y << endl;
    }
    //cout << "------IO_parse--------" << endl;
    for (size_t i = 0; i < IOs_parse->used_pinLocs.size(); i++)
    {
        //cout << IOs_parse->used_pinLocs[i]->x << ", " << IOs_parse->used_pinLocs[i]->y << endl;
        outfile << IOs_parse->used_pinLocs[i]->x << " " << IOs_parse->used_pinLocs[i]->y << endl;
    }
    outfile.close();
}

Coord dist_cal::cal_mid_coord()
{
    int x = (this->c1->x + this->c2->x) / 2;
    int y = (this->c1->y + this->c2->y) / 2;
    Coord new_coord(x, y);
    return new_coord;
}

void IO_parse::print_IO_pinLocs()
{
    for (size_t i = 0; i < placed_pinLocs.size(); i++)
    {
        Coord *temp_coord = placed_pinLocs[i];
        cout << temp_coord->x << ", " << temp_coord->y << endl;
    }
}

void IO_parse::gen_available_bp(vector<vector<Tile *>> vec_tile)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[i].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->Ubd)
                this->available_bp.push_back(temp_tile->Ucoord);
            if (temp_tile->Dbd)
                this->available_bp.push_back(temp_tile->Dcoord);
            if (temp_tile->Lbd)
                this->available_bp.push_back(temp_tile->Lcoord);
            if (temp_tile->Rbd)
                this->available_bp.push_back(temp_tile->Rcoord);
        }
    }
}

void IO_parse::print_available_bp()
{

    for (size_t i = 0; i < this->available_bp.size(); i++)
    {
        this->available_bp[i]->print_self();
    }
}

void IO_parse::use_bp(Coord *bp_coord)
{
    for (size_t i = 0; i < this->available_bp.size(); i++)
    {
        if (bp_coord->is_equal(this->available_bp[i]))
        {
            this->used_bp.push_back(bp_coord);
            this->available_bp.erase(this->available_bp.begin() + i);
        }
    }
}

void Net::convert_coordIO_to_coordBp(vector<IO *> vec_IO)
{
    for (size_t i = 0; i < this->coord_pins.size(); i++)
    {
        Coord *temp_coord_net = this->coord_pins[i];
        for (size_t j = 0; j < vec_IO.size(); j++)
        {
            IO *temp_IO = vec_IO[j];
            Coord *temp_coord_IO = temp_IO->coord;
            if (temp_coord_net->is_equal(temp_coord_IO))
            {
                Coord *temp_coord_bp = temp_IO->BPptr->coord;
                this->coord_pins[i] = temp_coord_bp;
                //cout << "convert_coordIO_to_coordBp: ";
                //temp_coord_IO->print_self();
                //cout << " to ";
                //temp_coord_bp->print_self();
                return;
            }
        }
    }

    //cout << endl;
}

void convert_coordIO_to_coordBp_for_vec_nets(vector<Net *> vec_net, vector<IO *> vec_IO)
{
    //int count = 0;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (!temp_net->is_IO_connection)
            continue;
        temp_net->convert_coordIO_to_coordBp(vec_IO);
        //cout << count++ << endl;
    }
}

bool Chiplet::is_in_chiplet(Tile *tile)
{
    if ((tile->x_mid > this->x) && (tile->x_mid < this->x + this->width) &&
        (tile->y_mid > this->y) && (tile->y_mid < this->y + this->height))
        return true;
    else
        return false;
}

bool Chiplet::is_coord_in_chiplet(Coord *coord)
{
    if ((coord->x > this->x) && (coord->x < this->x + this->width) &&
        (coord->y > this->y) && (coord->y < this->y + this->height))
        return true;

    else
        return false;
}

void output_csv_files(vector<Edge *> vec_edge, vector<Node *> vec_node, int num_src, int demand_supply)
{
    ofstream arcFrom, arcMax, arcMin, arcTo, arcWeight, nodeID, nodeSD, node_arc_Num;
    arcFrom.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcFrom.csv");
    if (!arcFrom.is_open())
        cout << "error: arcFrom.csv is not opened." << endl;
    arcFrom << "FROM_ID,Padding\n";
    arcMax.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcMax.csv");
    arcMax << "CAPACITY,Padding\n";
    arcMin.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcMin.csv");
    arcMin << "LOWER,Padding\n";
    arcTo.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcTo.csv");
    arcTo << "TO_ID,Padding\n";
    arcWeight.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcWeight.csv");
    arcWeight << "COST,Padding\n";
    nodeID.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeID.csv");
    nodeID << "NODE_ID,Padding\n";
    nodeSD.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeSD.csv");
    nodeSD << "SUPPLY,Padding\n";
    node_arc_Num.open("../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeNum_arcNum.csv");
    node_arc_Num << "nodeNum,arcNum\n";

    for (size_t i = 0; i < vec_edge.size(); i++)
    {
        Edge *temp_edge = vec_edge[i];
        Node *temp_node_from = temp_edge->src;
        Node *temp_node_to = temp_edge->tar;
        if (i == vec_edge.size() - 1)
        {
            arcFrom << temp_node_from->idx << ",0";
            arcTo << temp_node_to->idx << ",0";
            arcMax << temp_edge->capacity << ",0";
            arcMin << "0,0";
            arcWeight << temp_edge->cost << ",0";
        }
        else
        {

            arcFrom << temp_node_from->idx << ",0\n";
            arcTo << temp_node_to->idx << ",0\n";
            arcMax << temp_edge->capacity << ",0\n";
            arcMin << "0,0\n";
            arcWeight << temp_edge->cost << ",0\n";
        }
    }

    for (size_t i = 0; i < vec_node.size(); i++)
    {
        Node *temp_node = vec_node[i];
        if (i == vec_node.size() - 1)
        {
            nodeID << temp_node->idx << ",0";
            if (temp_node->idx == 0)
                nodeSD << demand_supply << ",0";
            else if (temp_node->idx == 1)
                nodeSD << -demand_supply << ",0";
            else
                nodeSD << "0,0";
        }
        else
        {
            nodeID << temp_node->idx << ",0\n";
            if (temp_node->idx == 0)
                nodeSD << demand_supply << ",0\n";
            else if (temp_node->idx == 1)
                nodeSD << -demand_supply << ",0\n";
            else
                nodeSD << "0,0\n";
        }
    }

    node_arc_Num << vec_node.size() << "," << vec_edge.size();

    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcFrom.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcMax.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcMin.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcTo.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/arcWeight.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeID.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeSD.csv" << endl;
    cout << "[file output]"
         << "../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/nodeNum_arcNum.csv" << endl;

    arcFrom.close();
    arcMax.close();
    arcMin.close();
    arcTo.close();
    arcWeight.close();
    nodeID.close();
    nodeSD.close();
    node_arc_Num.close();
}

void Net::add_to_used_mbs(MazeBlock *mb)
{
    this->used_mbs.push_back(mb);
}

void set_demand_supply(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->coord_pins.size() > 2)
            continue;
        int chiplet_idx1 = temp_net->pinnames_structure[0]->idx_chiplet;
        int chiplet_idx2 = temp_net->pinnames_structure[1]->idx_chiplet;
        if (chiplet_idx1 != (int)(vec_chiplet.size()) && chiplet_idx2 != (int)(vec_chiplet.size())) //!! not generalized...chiplet 0 is always the source pins
        {
            if (chiplet_idx1 == 0 && chiplet_idx2 != 0)
            {
                vec_chiplet[chiplet_idx1]->num_s++;
                vec_chiplet[chiplet_idx2]->num_t++;
            }
            else if (chiplet_idx1 != 0 && chiplet_idx2 == 0)
            {
                vec_chiplet[chiplet_idx1]->num_t++;
                vec_chiplet[chiplet_idx2]->num_s++;
            }
        }
        else if (chiplet_idx1 == (int)(vec_chiplet.size()))
        {
            //vec_chiplet[chiplet_idx1]->num_t++;
            vec_chiplet[chiplet_idx2]->num_s++;
        }
        else if (chiplet_idx2 == (int)(vec_chiplet.size()))
        {
            //vec_chiplet[chiplet_idx2]->num_t++;
            vec_chiplet[chiplet_idx1]->num_s++;
        }
    }
}

int get_demand_supply(vector<Chiplet *> vec_chiplet)
{
    int demand_supply = 0;
    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        Chiplet *temp_chiplet = vec_chiplet[i];
        demand_supply += temp_chiplet->num_s;
    }
    return demand_supply;
}

void output_coord3(ofstream &outfile, Coord3 *coord)
{
    outfile << "(" << coord->x << ", " << coord->y << ", " << coord->h << ")";
}

char get_closest_side(Coord3 *coord3, int W_FP, int H_FP)
{
    vector<pair<int, char>> vec_pair;

    //to U
    int dist_to_U = abs(H_FP - coord3->y);
    pair<int, char> pair_U = make_pair(dist_to_U, 'U');
    vec_pair.push_back(pair_U);

    //to D
    int dist_to_D = abs(0 - coord3->y);
    pair<int, char> pair_D = make_pair(dist_to_D, 'D');
    vec_pair.push_back(pair_D);

    //to L
    int dist_to_L = abs(0 - coord3->x);
    pair<int, char> pair_L = make_pair(dist_to_L, 'L');
    vec_pair.push_back(pair_L);

    //to R
    int dist_to_R = abs(W_FP - coord3->x);
    pair<int, char> pair_R = make_pair(dist_to_R, 'R');
    vec_pair.push_back(pair_R);

    sort(vec_pair.begin(), vec_pair.end(), [](const pair<int, char> &lhs, const pair<int, char> &rhs)
         { return lhs.first < rhs.first; });

    //return the char with smallest distance to the FP boundary
    //cout << vec_pair.front().second << endl;
    return vec_pair.front().second;
}

Coord3 *get_extended_coord3_to_IOpad(Coord3 *coord_bp, char side, int dist_to_fp_boundary, int W_FP, int H_FP)
{

    if (side == 'D')
    {
        Coord3 *new_coord = new Coord3(coord_bp->x, dist_to_fp_boundary, coord_bp->h);
        return new_coord;
    }
    else if (side == 'U')
    {
        Coord3 *new_coord = new Coord3(coord_bp->x, H_FP - dist_to_fp_boundary, coord_bp->h);
        return new_coord;
    }
    else if (side == 'L')
    {
        Coord3 *new_coord = new Coord3(dist_to_fp_boundary, coord_bp->y, coord_bp->h);
        return new_coord;
    }
    else if (side == 'R')
    {
        Coord3 *new_coord = new Coord3(W_FP - dist_to_fp_boundary, coord_bp->y, coord_bp->h);
        return new_coord;
    }
    else
    {
        cout << "error: no such a char" << endl;
        return nullptr;
    }
}

void output_routing_path(vector<vector<Coord3 *>> vec_coord3_uncleaned, vector<BP *> allocated_bp, string Filename, int W_FP, int H_FP, int pad_size)
{
    ofstream outfile;
    outfile.open(Filename);
    if (!outfile.is_open())
    {
        cout << "error: cannot open output file " << Filename << endl;
        return;
    }
    bool is_bp = false;
    for (size_t i = 0; i < vec_coord3_uncleaned.size(); i++)
    {
        outfile << "Path#" << i << ": ";
        vector<Coord3 *> vec_coord3 = vec_coord3_uncleaned[i];
        Coord3 *back_coord = nullptr, *mid_coord = nullptr, *front_coord = nullptr;
        int WL = 0, via_count = 0;
        for (size_t j = 0; j < vec_coord3.size(); j++)
        {
            front_coord = vec_coord3[j];
            if (mid_coord != nullptr)
            {
                WL += front_coord->get_dist(mid_coord);
                if (front_coord->layerchange(mid_coord))
                    via_count++;
            }

            //reaches last element
            if (j == vec_coord3.size() - 1)
            {
                //check if is bp
                for (size_t k = 0; k < allocated_bp.size(); k++)
                {
                    Tile *temp_tile = allocated_bp[k]->parent_tile;
                    Coord *temp_coord = temp_tile->coord_mid;
                    if (front_coord->x == temp_coord->x && front_coord->y == temp_coord->y)
                        is_bp = true;
                }
                //when there is more than two points in this path
                if ((front_coord != nullptr) && (mid_coord != nullptr) && (back_coord != nullptr))
                {
                    if (front_coord->is_straitline(mid_coord, back_coord))
                    {
                        if (is_bp)
                        {
                            char side = get_closest_side(front_coord, W_FP, H_FP);
                            Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);

                            if (new_coord3_IOpad->is_straitline(front_coord, back_coord))
                            {
                                output_coord3(outfile, back_coord);
                                outfile << " - ";
                                output_coord3(outfile, new_coord3_IOpad);
                            }
                            else
                            {
                                output_coord3(outfile, back_coord);
                                outfile << " - ";
                                output_coord3(outfile, front_coord);
                                outfile << " - ";
                                output_coord3(outfile, new_coord3_IOpad);
                            }
                        }
                        else
                        {
                            output_coord3(outfile, back_coord);
                            outfile << " - ";
                            output_coord3(outfile, front_coord);
                        }
                        // if (is_bp)
                        // {
                        //     char side = get_closest_side(front_coord, W_FP, H_FP);
                        //     Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                        //     output_coord3(outfile, new_coord3_IOpad);
                        // }
                        continue;
                    }
                    else
                    {
                        if (is_bp)
                        {
                            char side = get_closest_side(front_coord, W_FP, H_FP);
                            Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                            if (new_coord3_IOpad->is_straitline(front_coord, mid_coord))
                            {
                                output_coord3(outfile, back_coord);
                                outfile << " - ";
                                output_coord3(outfile, mid_coord);
                                outfile << " - ";
                                output_coord3(outfile, new_coord3_IOpad);
                            }
                            else
                            {
                                output_coord3(outfile, back_coord);
                                outfile << " - ";
                                output_coord3(outfile, mid_coord);
                                outfile << " - ";
                                output_coord3(outfile, front_coord);
                                outfile << " - ";
                                output_coord3(outfile, new_coord3_IOpad);
                            }
                        }
                        else
                        {
                            output_coord3(outfile, back_coord);
                            outfile << " - ";
                            output_coord3(outfile, mid_coord);
                            outfile << " - ";
                            output_coord3(outfile, front_coord);
                        }
                        continue;
                    }
                }
                //when there is only two points (one segment) in this path
                else if ((front_coord != nullptr) && (mid_coord != nullptr) && (back_coord == nullptr))
                {
                    if (is_bp)
                    {
                        output_coord3(outfile, mid_coord);
                        outfile << " - ";
                        char side = get_closest_side(front_coord, W_FP, H_FP);
                        Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                        output_coord3(outfile, new_coord3_IOpad);
                    }
                    else
                    {
                        output_coord3(outfile, mid_coord);
                        outfile << " - ";
                        output_coord3(outfile, front_coord);
                    }
                    continue;
                }
            }
            //

            if ((front_coord != nullptr) && (back_coord != nullptr) && (mid_coord != nullptr))
            {
                if (front_coord->is_straitline(mid_coord, back_coord))
                {
                    mid_coord = front_coord;
                    continue;
                }
            }

            if (back_coord != nullptr)
            {
                output_coord3(outfile, back_coord);
                outfile << " - ";
            }
            back_coord = mid_coord;
            mid_coord = front_coord;
        }
        outfile << endl;
        outfile << "WL: " << WL << " Via#: " << via_count << " is_IO_net: " << is_bp;
        is_bp = false;
        if (i != vec_coord3_uncleaned.size() - 1)
            outfile << endl;
    }
}

void output_3_pin_nets(vector<Net *> vec_net, int wire_spacing, string filename_maze_route_output)
{
    ofstream outfile;
    outfile.open(filename_maze_route_output);
    if (!(outfile.is_open()))
    {
        cout << "error: cannot open file" << filename_maze_route_output << endl;
        return;
    }
    int idx_multipin_net = 0;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->coord_pins.size() < 3)
            continue;

        outfile << "net " << temp_net->netname << " net# " << idx_multipin_net++ << endl;
        MazeBlock *temp_mb = nullptr, *temp_mb_prev = nullptr;
        for (size_t j = 0; j < temp_net->used_mbs.size(); j++)
        {
            temp_mb = temp_net->used_mbs[j];
            if (temp_mb_prev != nullptr)
            {
                if (j != 0 && (temp_mb->is_neighbor(temp_mb_prev, wire_spacing)))
                    outfile << " - ";
                if (!(temp_mb->is_neighbor(temp_mb_prev, wire_spacing)))
                {
                    outfile << endl;
                }
            }
            outfile << "(" << temp_mb->coord_mid->x << ", " << temp_mb->coord_mid->y << ", " << temp_mb->idx_h << ")";
            // if (j != temp_net->used_mbs.size() - 1)
            //     outfile << " - ";

            temp_mb_prev = temp_mb;
        }
        outfile << endl;
    }
}
/**
 * @brief 
 * 
 * @param vec_net 
 * @param wire_spacing 
 * @param filename_maze_route_output 
 */
void output_3_pin_nets_mr3(vector<Net *> vec_net, int wire_spacing, string filename_maze_route_output)
{
    ofstream outfile;
    outfile.open(filename_maze_route_output);
    if (!(outfile.is_open()))
    {
        cout << "error: cannot open file" << filename_maze_route_output << endl;
        return;
    }
    int idx_multipin_net = 0;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->coord_pins.size() < 3)
            continue;

        outfile << "net " << temp_net->netname << " net# " << idx_multipin_net++ << endl;
        MazeBlock *temp_mb = nullptr, *temp_mb_prev = nullptr;
        for (size_t j = 0; j < temp_net->used_mbs.size(); j++)
        {
            temp_mb = temp_net->used_mbs[j];
            if (temp_mb_prev != nullptr)
            {
                if (j != 0 && (temp_mb->is_neighbor(temp_mb_prev, wire_spacing)))
                    outfile << " - ";
                if (!(temp_mb->is_neighbor(temp_mb_prev, wire_spacing)))
                {
                    outfile << endl;
                }
            }
            outfile << "(" << temp_mb->coord_mid->x << ", " << temp_mb->coord_mid->y << ", " << temp_mb->idx_h << ")";
            // if (j != temp_net->used_mbs.size() - 1)
            //     outfile << " - ";

            temp_mb_prev = temp_mb;
        }
        outfile << endl;
    }
}

void write_path(vector<Path *> vec_path, vector<BP *> allocated_bp, int W_FP, int H_FP, int pad_size)
{
    bool is_bp = false;
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        //cout << "[wrte path]" << vec_path[i]->netname << endl;
        Path *temp_path = vec_path[i];
        vector<Coord3 *> vec_coord3_uncleaned = temp_path->coord3_path_uncleaned;
        vector<Coord3 *> vec_coord3_cleaned; // = temp_path->coord3_path_cleaned;

        Coord3 *back_coord = nullptr, *mid_coord = nullptr, *front_coord = nullptr;
        int WL = 0, via_count = 0;
        for (size_t j = 0; j < vec_coord3_uncleaned.size(); j++)
        {
            front_coord = vec_coord3_uncleaned[j];
            if (mid_coord != nullptr)
            {
                WL += front_coord->get_dist(mid_coord);
                if (front_coord->layerchange(mid_coord))
                    via_count++;
            }

            //reaches last element
            if (j == vec_coord3_uncleaned.size() - 1)
            {
                //check if is bp
                for (size_t k = 0; k < allocated_bp.size(); k++)
                {
                    Tile *temp_tile = allocated_bp[k]->parent_tile;
                    Coord *temp_coord = temp_tile->coord_mid;
                    if (front_coord->x == temp_coord->x && front_coord->y == temp_coord->y)
                        is_bp = true;
                }
                //when there is more than two points in this path
                if ((front_coord != nullptr) && (mid_coord != nullptr) && (back_coord != nullptr))
                {
                    if (front_coord->is_straitline(mid_coord, back_coord))
                    {
                        if (is_bp)
                        {
                            char side = get_closest_side(front_coord, W_FP, H_FP);
                            Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);

                            if (new_coord3_IOpad->is_straitline(front_coord, back_coord))
                            {
                                vec_coord3_cleaned.push_back(back_coord);
                                vec_coord3_cleaned.push_back(new_coord3_IOpad);
                            }
                            else
                            {
                                vec_coord3_cleaned.push_back(back_coord);
                                vec_coord3_cleaned.push_back(front_coord);
                                vec_coord3_cleaned.push_back(new_coord3_IOpad);
                            }
                        }
                        else
                        {
                            vec_coord3_cleaned.push_back(back_coord);
                            vec_coord3_cleaned.push_back(front_coord);
                        }
                        // if (is_bp)
                        // {
                        //     char side = get_closest_side(front_coord, W_FP, H_FP);
                        //     Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                        //     output_coord3(outfile, new_coord3_IOpad);
                        // }
                        continue;
                    }
                    else
                    {
                        if (is_bp)
                        {
                            char side = get_closest_side(front_coord, W_FP, H_FP);
                            Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                            if (new_coord3_IOpad->is_straitline(front_coord, mid_coord))
                            {
                                vec_coord3_cleaned.push_back(back_coord);
                                vec_coord3_cleaned.push_back(mid_coord);
                                vec_coord3_cleaned.push_back(new_coord3_IOpad);
                            }
                            else
                            {
                                vec_coord3_cleaned.push_back(back_coord);
                                vec_coord3_cleaned.push_back(mid_coord);
                                vec_coord3_cleaned.push_back(front_coord);
                                vec_coord3_cleaned.push_back(new_coord3_IOpad);
                            }
                        }
                        else
                        {
                            vec_coord3_cleaned.push_back(back_coord);
                            vec_coord3_cleaned.push_back(mid_coord);
                            vec_coord3_cleaned.push_back(front_coord);
                        }
                        continue;
                    }
                }
                //when there is only two points (one segment) in this path
                else if ((front_coord != nullptr) && (mid_coord != nullptr) && (back_coord == nullptr))
                {
                    if (is_bp)
                    {
                        vec_coord3_cleaned.push_back(mid_coord);
                        char side = get_closest_side(front_coord, W_FP, H_FP);
                        Coord3 *new_coord3_IOpad = get_extended_coord3_to_IOpad(front_coord, side, pad_size / 2, W_FP, H_FP);
                        vec_coord3_cleaned.push_back(new_coord3_IOpad);
                    }
                    else
                    {
                        vec_coord3_cleaned.push_back(mid_coord);
                        vec_coord3_cleaned.push_back(front_coord);
                    }
                    continue;
                }
            }
            //

            if ((front_coord != nullptr) && (back_coord != nullptr) && (mid_coord != nullptr))
            {
                if (front_coord->is_straitline(mid_coord, back_coord))
                {
                    mid_coord = front_coord;
                    continue;
                }
            }

            if (back_coord != nullptr)
            {
                vec_coord3_cleaned.push_back(back_coord);
            }
            back_coord = mid_coord;
            mid_coord = front_coord;
        }
        //outfile << "WL: " << WL << " Via#: " << via_count << " is_IO_net: " << is_bp;
        temp_path->WL = WL;
        temp_path->via_count = via_count;
        temp_path->coord3_path_cleaned = vec_coord3_cleaned;
        is_bp = false;
    }
    cout << endl;
}

//divide paths into 2-pin and multi-pin nets
void divide_paths(vector<Path *> vec_path_original, vector<Path *> &vec_path_2pin, vector<Path *> &vec_path_multipin)
{
    for (size_t i = 0; i < vec_path_original.size(); i++)
    {
        Path *temp_path = vec_path_original[i];
        if (!temp_path->is_multiPin)
        {
            // cout << temp_path->netname << " " << temp_path->coord3_path_cleaned.size() << endl;
            vec_path_2pin.push_back(temp_path);
        }
        else
            vec_path_multipin.push_back(temp_path);
    }
}

void sort_2pin_paths_by_ascending_WL(vector<Path *> &vec_path_2pin)
{
    sort(vec_path_2pin.begin(), vec_path_2pin.end(), [](Path *const &left, Path *const &right)
         { return left->WL < right->WL; });
}

void sort_2pin_paths_by_descending_WL(vector<Path *> &vec_path_2pin)
{
    sort(vec_path_2pin.begin(), vec_path_2pin.end(), [](Path *const &left, Path *const &right)
         { return left->WL > right->WL; });
}

bool is_idx_chiplet_pair_match_pin_pair(int idx_chiplet1, int idx_chiplet2, int idx_chiplet_pin1, int idx_chiplet_pin2)
{
    return (idx_chiplet1 == idx_chiplet_pin1 && idx_chiplet2 == idx_chiplet_pin2) || (idx_chiplet1 == idx_chiplet_pin2 && idx_chiplet2 == idx_chiplet_pin1);
}

void read_paths_into_nets(vector<Net *> vec_net, vector<Path *> vec_path_2pin, vector<Path *> vec_path_multipin, vector<IO *> vec_IO, vector<Chiplet *> vec_chiplet)
{
    int index_path_2pin_IO = 0;
    int index_path_2pin_nonIO_0to1 = 0;
    int index_path_2pin_nonIO_0to2 = 0;
    int index_path_multipin = 0;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        string net_name = temp_net->netname;
        //cout << "net #" << i << " netname: " << net_name << endl;

        //2 pin nets
        if (temp_net->get_num_pins() == 2)
        {
            if (temp_net->is_IO_connection)
            {
                Path *temp_path = vec_path_2pin[index_path_2pin_IO];
                //cout << temp_path->netname << endl;
                while (!temp_path->is_IO) //skip those non-IO nets
                {
                    temp_path = vec_path_2pin[++index_path_2pin_IO];
                }
                vector<Coord3 *> vec_coord3_cleaned = temp_path->coord3_path_cleaned;
                temp_net->vec_routing_path_coord3_cleaned.push_back(vec_coord3_cleaned);
                //temp_net->assign_path(temp_path);
                index_path_2pin_IO++;
            }
            else
            {
                int idx_chiplet_of_pin1 = temp_net->pinnames_structure[0]->idx_chiplet;
                int idx_chiplet_of_pin2 = temp_net->pinnames_structure[1]->idx_chiplet;
                Path *temp_path = nullptr;
                int case_num = -1;
                if ((idx_chiplet_of_pin1 == 0 && idx_chiplet_of_pin2 == 1) || (idx_chiplet_of_pin1 == 1 && idx_chiplet_of_pin2 == 0))
                {
                    temp_path = vec_path_2pin[index_path_2pin_nonIO_0to1];
                    case_num = 1;
                }
                else if ((idx_chiplet_of_pin1 == 0 && idx_chiplet_of_pin2 == 2) || (idx_chiplet_of_pin1 == 2 && idx_chiplet_of_pin2 == 0))
                {
                    temp_path = vec_path_2pin[index_path_2pin_nonIO_0to2];
                    case_num = 2;
                }

                int idx_chiplet1 = -1, idx_chiplet2 = -1;
                //bool flag_matching_idx_chiplet = false;

                switch (case_num)
                {
                case 1:
                    while (temp_path->is_IO)
                        temp_path = vec_path_2pin[++index_path_2pin_nonIO_0to1];

                    idx_chiplet1 = temp_path->get_idx_chiplet_for_first_rdl_in_path(vec_chiplet);
                    idx_chiplet2 = temp_path->get_idx_chiplet_for_last_rdl_in_path(vec_chiplet);

                    while (!is_idx_chiplet_pair_match_pin_pair(idx_chiplet_of_pin1, idx_chiplet_of_pin2, idx_chiplet1, idx_chiplet2))
                    {
                        temp_path = vec_path_2pin[++index_path_2pin_nonIO_0to1];
                        idx_chiplet1 = temp_path->get_idx_chiplet_for_first_rdl_in_path(vec_chiplet);
                        idx_chiplet2 = temp_path->get_idx_chiplet_for_last_rdl_in_path(vec_chiplet);
                    }

                    index_path_2pin_nonIO_0to1++; //prepare for next iteration
                    break;
                case 2:
                    while (temp_path->is_IO)
                        temp_path = vec_path_2pin[++index_path_2pin_nonIO_0to2];

                    idx_chiplet1 = temp_path->get_idx_chiplet_for_first_rdl_in_path(vec_chiplet);
                    idx_chiplet2 = temp_path->get_idx_chiplet_for_last_rdl_in_path(vec_chiplet);

                    while (!is_idx_chiplet_pair_match_pin_pair(idx_chiplet_of_pin1, idx_chiplet_of_pin2, idx_chiplet1, idx_chiplet2))
                    {
                        temp_path = vec_path_2pin[++index_path_2pin_nonIO_0to2];
                        idx_chiplet1 = temp_path->get_idx_chiplet_for_first_rdl_in_path(vec_chiplet);
                        idx_chiplet2 = temp_path->get_idx_chiplet_for_last_rdl_in_path(vec_chiplet);
                    }

                    index_path_2pin_nonIO_0to2++; //prepare for next iteration
                    break;
                default:
                    break;
                }
                //cout << "net id1, id2: " << idx_chiplet_of_pin1 << ", " << idx_chiplet_of_pin2 << endl;
                //cout << "chiplet id1, id2: " << idx_chiplet1 << ", " << idx_chiplet2 << endl;

                vector<Coord3 *> vec_coord3_cleaned = temp_path->coord3_path_cleaned;
                temp_net->vec_routing_path_coord3_cleaned.push_back(vec_coord3_cleaned);
                //temp_net->assign_path(temp_path);
                //index_path_2pin_nonIO++;
            }
        }
        //multi-pin nets
        else
        {

            for (size_t j = 0; j < vec_path_multipin.size(); j++)
            {
                Path *temp_path = vec_path_multipin[j];
                string net_name_path = temp_path->netname;
                vector<Coord3 *> vec_coord3_cleaned = temp_path->coord3_path_cleaned;
                if (net_name == net_name_path)
                {
                    temp_net->vec_routing_path_coord3_cleaned.push_back(vec_coord3_cleaned);
                    //temp_net->assign_path(temp_path);
                }
            }
            index_path_multipin++;
        }
    }
    //assign netname to IOs
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->is_IO_connection)
        {
            //cout << "this is IO, netname: " << temp_net->netname << endl;
            temp_net->set_parent_net_of_IO(vec_IO);
        }
    }
}

void output_coord3_pair_to_json_format(ofstream &outfile, Coord3 *c3_1, Coord3 *c3_2)
{
    outfile << "[";
    outfile << "[" << c3_1->x << ", " << c3_1->y << ", " << c3_1->h << "],";
    outfile << "[" << c3_2->x << ", " << c3_2->y << ", " << c3_2->h << "]";
    outfile << "]";
}

void output_solution_to_json(char *input_json_filename, string filename_json, vector<Net *> vec_net)
{
    // ifstream infile(input_json_filename);
    // json inJson;
    // infile >> inJson;

    ofstream outfile;
    outfile.open(filename_json);
    if (!outfile.is_open())
    {
        cout << "error: cannot open file \"" << filename_json << "\"" << endl;
        return;
    }
    outfile << "\"RoutingSolution\": {" << endl;

    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        outfile << "\"" << temp_net->netname << "\""
                << ": [" << endl;
        for (size_t j = 0; j < temp_net->vec_routing_path_coord3_cleaned.size(); j++)
        {
            vector<Coord3 *> vec_coord3 = temp_net->vec_routing_path_coord3_cleaned[j];
            Coord3 *cur_c3 = nullptr, *prev_c3 = nullptr;
            for (size_t k = 0; k < vec_coord3.size(); k++)
            {
                cur_c3 = vec_coord3[k];
                if (!k == 0)
                {
                    if (!(k == vec_coord3.size() - 1))
                    {
                        output_coord3_pair_to_json_format(outfile, prev_c3, cur_c3);
                        outfile << "," << endl;
                    }
                    else
                    {
                        if (j == (temp_net->vec_routing_path_coord3_cleaned.size() - 1))
                        {
                            output_coord3_pair_to_json_format(outfile, prev_c3, cur_c3);
                            outfile << endl;
                        }
                        else
                        {
                            output_coord3_pair_to_json_format(outfile, prev_c3, cur_c3);
                            outfile << "," << endl;
                        }
                    }
                }
                else if (k == vec_coord3.size() - 1)
                    continue;

                prev_c3 = cur_c3;
            }
        }

        if (i != vec_net.size() - 1)
            outfile << "]," << endl;
        else
        {
            outfile << "]" << endl;
            outfile << "}";
        }
    }
}

void output_solution_to_json_2(char *input_json_filename, vector<Net *> vec_net, vector<IO *> vec_IO)
{
    ifstream infile(input_json_filename);
    json inJson;
    infile >> inJson;

    string output_filename = input_json_filename;
    output_filename.erase(output_filename.end() - 5, output_filename.end());
    output_filename.erase(output_filename.begin(), output_filename.begin() + 9);
    output_filename = "../../output/" + output_filename;
    //output IO location
    //cout << "[io count] " << vec_IO.size() << endl;
    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        IO *temp_io = vec_IO[i];
        string netname = temp_io->parent_net->netname;
        //cout << "[temp_io->netname]" << temp_io->parent_net->netname << endl;
        int x_low = temp_io->x - temp_io->pad_size / 2;
        int y_low = temp_io->y - temp_io->pad_size / 2;
        int x_high = temp_io->x + temp_io->pad_size / 2;
        int y_high = temp_io->y + temp_io->pad_size / 2;

        inJson["Ext_IO_Location"][netname] = {{x_low, y_low}, {x_high, y_high}};
        // cout << temp_io->BPptr->coord->x << endl;
        // cout << temp_io->BPptr->parent_tile->height << endl;
        // cout << "[temp_io->BPptr->parent_tile->net->is_IO_connection] " << temp_io->BPptr->parent_tile->net->is_IO_connection << endl;
    }

    //output routing solution
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        //int max_num_segment = temp_net->vec_routing_path_coord3_cleaned.size();
        int idx_segment = 0;
        for (size_t j = 0; j < temp_net->vec_routing_path_coord3_cleaned.size(); j++)
        {
            vector<Coord3 *> vec_coord3 = temp_net->vec_routing_path_coord3_cleaned[j];
            Coord3 *cur_c3 = nullptr, *prev_c3 = nullptr;
            for (size_t k = 0; k < vec_coord3.size(); k++)
            {
                cur_c3 = vec_coord3[k];
                if (k != 0)
                {
                    inJson["RoutingSolution"][temp_net->netname][idx_segment][0][0] = prev_c3->x;
                    inJson["RoutingSolution"][temp_net->netname][idx_segment][0][1] = prev_c3->y;
                    inJson["RoutingSolution"][temp_net->netname][idx_segment][0][2] = prev_c3->h;

                    //([prev_c3->x, prev_c3->y, prev_c3->h])

                    inJson["RoutingSolution"][temp_net->netname][idx_segment][1][0] = cur_c3->x;
                    inJson["RoutingSolution"][temp_net->netname][idx_segment][1][1] = cur_c3->y;
                    inJson["RoutingSolution"][temp_net->netname][idx_segment][1][2] = cur_c3->h;

                    idx_segment++;

                    if (k == vec_coord3.size() - 1 && temp_net->coord_pins.size() == 2)
                    {
                        // cout << " final pin: ";
                        // cur_c3->self_print();
                        Coord *temp_coord = new Coord(cur_c3->x, cur_c3->y);
                        temp_net->assigned_coordinate.push_back(temp_coord);
                        //string netname = find_coord_n_signal_in_net(temp_coord, vec_net);
                        //cout << " " << netname << endl;
                    }
                }
                else if (k == vec_coord3.size() - 1)
                {

                    continue;
                }
                else if (k == 0 && temp_net->coord_pins.size() == 2)
                {
                    // cout << "first pin: ";

                    // cur_c3->self_print();
                    Coord *temp_coord = new Coord(cur_c3->x, cur_c3->y);
                    temp_net->assigned_coordinate.push_back(temp_coord);
                    //string netname = find_coord_n_signal_in_net(temp_coord, vec_net);
                    //cout << " " << netname << " isIO: " << temp_net->is_IO_connection;
                    //continue;
                }
                prev_c3 = cur_c3;
            }
        }
    }
    output_filename = output_filename + "-output.json";
    std::ofstream o(output_filename);
    o << std::setw(4) << inJson << std::endl;
    cout << "[file output] " << output_filename << endl;
    o.close();
}

void Net::set_parent_net_of_IO(vector<IO *> vec_IO)
{
    for (size_t i = 0; i < this->vec_routing_path_coord3_cleaned.size(); i++)
    {
        vector<Coord3 *> temp_vec_coord3 = this->vec_routing_path_coord3_cleaned[i];
        for (size_t j = 0; j < temp_vec_coord3.size(); j++)
        {
            Coord3 *temp_coord3 = temp_vec_coord3[j];

            bool flag = false;
            for (size_t k = 0; k < vec_IO.size(); k++)
            {
                IO *temp_IO = vec_IO[k];
                flag = temp_IO->is_Coord3_in_IO(temp_coord3);
                if (flag)
                {
                    temp_IO->parent_net = this;
                    //cout << "[IO's netname]" << this->netname << endl;
                    return;
                }
            }
        }
    }
}

void Chiplet::gen_placed_pinLocs_n_used_pinLocs()
{
    for (size_t i = 0; i < this->placed_pinLocs.size(); i++)
    {
        Coord *temp_coord = this->placed_pinLocs[i];
        this->placed_pinLocs_n_used_pinLocs.push_back(temp_coord);
    }

    for (size_t i = 0; i < this->used_pinLocs.size(); i++)
    {
        Coord *temp_coord = this->used_pinLocs[i];
        this->placed_pinLocs_n_used_pinLocs.push_back(temp_coord);
    }

    //cout << "this->placed_pinLocs_n_used_pinLocs has " << this->placed_pinLocs_n_used_pinLocs.size() << endl;
}

string find_coord_n_signal_in_net(Coord *coord, vector<Net *> vec_net)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        //2-pin nets
        if (temp_net->coord_pins.size() == 2)
            for (size_t j = 0; j < temp_net->assigned_coordinate.size(); j++)
            {
                Coord *temp_coord = temp_net->assigned_coordinate[j];
                if (coord->is_equal(temp_coord))
                {
                    return temp_net->netname;
                }
            }

        //multipin nets
        else
        {
            for (size_t j = 0; j < temp_net->coord_pins.size(); j++)
            {
                Coord *temp_coord = temp_net->coord_pins[j];
                if (coord->is_equal(temp_coord))
                {
                    return temp_net->netname;
                }
            }
        }
    }
    // cout << "error: cannot find this coordinate ";
    // coord->print_self();
    // cout << " in the netlist" << endl;
    return "SKIP";
}

string find_coord_n_signal_in_net_mr3(Coord *coord, vector<Net *> vec_net)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        //2-pin nets
        if (temp_net->coord_pins.size() == 2)
            for (size_t j = 0; j < temp_net->assigned_coordinate.size(); j++)
            {
                Coord *temp_coord = temp_net->assigned_coordinate[j];
                if (coord->is_equal(temp_coord))
                {
                    //cout << temp_net->netname << " tb:" << temp_net->delay << " wl:" << temp_net->assigned_path->WL << endl;
                    return temp_net->netname;
                }
            }

        //multipin nets
        else
        {
            for (size_t j = 0; j < temp_net->coord_pins_mr3.size(); j++)
            {
                Coord *temp_coord = temp_net->coord_pins_mr3[j];
                if (coord->is_equal(temp_coord))
                {
                    //cout << temp_net->netname << " tb:" << temp_net->delay << endl;
                    return temp_net->netname;
                }
            }
        }
    }
    // cout << "error: cannot find this coordinate ";
    // coord->print_self();
    // cout << " in the netlist" << endl;
    return "SKIP";
}

void Chiplet::set_max_pin_row_num()
{
    sort(this->unplaced_pinLocs.begin(), this->unplaced_pinLocs.end(), [](Coord *const &a, Coord *const &b)
         { return a->x < b->x; });
    int x_min = this->unplaced_pinLocs[0]->x;
    int count_x_min = 0;

    for (size_t i = 0; i < this->unplaced_pinLocs.size(); i++)
    {
        if (this->unplaced_pinLocs[i]->x == x_min)
            count_x_min++;
    }
    this->max_pin_row_num = count_x_min;
}

void Chiplet::set_max_pin_col_num()
{
    sort(this->unplaced_pinLocs.begin(), this->unplaced_pinLocs.end(), [](Coord *const &a, Coord *const &b)
         { return a->y > b->y; });
    int y_max = this->unplaced_pinLocs[0]->y;
    int count_y_max = 0;

    for (size_t i = 0; i < this->unplaced_pinLocs.size(); i++)
    {
        if (this->unplaced_pinLocs[i]->y == y_max)
            count_y_max++;
    }
    this->max_pin_col_num = count_y_max;
}

//find the Coord to output as a netname in CSV
Coord *Chiplet::get_n_erase_prior_coord()
{

    sort(this->placed_pinLocs_n_used_pinLocs.begin(), this->placed_pinLocs_n_used_pinLocs.end(), [](Coord *const &a, Coord *const &b)
         { return a->y > b->y; });
    int y_max = this->placed_pinLocs_n_used_pinLocs[0]->y;

    vector<Coord *> selected_coord;
    for (size_t i = 0; i < this->placed_pinLocs_n_used_pinLocs.size(); i++)
    {
        Coord *temp_coord = this->placed_pinLocs_n_used_pinLocs[i];
        if (temp_coord->y == y_max)
            selected_coord.push_back(temp_coord);
    }

    sort(selected_coord.begin(), selected_coord.end(), [](Coord *const &a, Coord *const &b)
         { return a->x < b->x; });

    for (size_t i = 0; i < this->placed_pinLocs_n_used_pinLocs.size(); i++)
    {
        Coord *temp_coord = this->placed_pinLocs_n_used_pinLocs[i];
        if (temp_coord == selected_coord[0])
        {
            this->placed_pinLocs_n_used_pinLocs.erase(this->placed_pinLocs_n_used_pinLocs.begin() + i);
            break;
        }
    }

    return selected_coord[0];
}

void output_signal_names_as_csv_table(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet)
{
    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        Chiplet *temp_chiplet = vec_chiplet[i];
        string filename_output = "../../output/pin_grid_" + temp_chiplet->name + ".csv";
        ofstream outfile(filename_output);
        if (!outfile.is_open())
        {
            cout << "error: " << filename_output << " cannot be opened for output" << endl;
            return;
        }
        temp_chiplet->gen_placed_pinLocs_n_used_pinLocs();
        temp_chiplet->set_max_pin_col_num();
        temp_chiplet->set_max_pin_row_num();
        for (int j = 0; j < temp_chiplet->max_pin_row_num; j++)
        {
            for (int k = 0; k < temp_chiplet->max_pin_col_num; k++)
            {
                Coord *coord_to_output = temp_chiplet->get_n_erase_prior_coord();
                string netname_to_output = find_coord_n_signal_in_net(coord_to_output, vec_net);
                outfile << netname_to_output;
                if (k != temp_chiplet->max_pin_col_num - 1)
                    outfile << ",";
            }
            outfile << endl;
        }
        cout << "[output file] " << filename_output << endl;
        outfile.close();
    }
}

void output_signal_names_as_csv_table_mr3(vector<Net *> vec_net, vector<Chiplet *> vec_chiplet)
{
    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        Chiplet *temp_chiplet = vec_chiplet[i];
        string filename_output = "../../output/pin_grid_" + temp_chiplet->name + ".csv";
        ofstream outfile(filename_output);
        if (!outfile.is_open())
        {
            cout << "error: " << filename_output << " cannot be opened for output" << endl;
            return;
        }
        temp_chiplet->gen_placed_pinLocs_n_used_pinLocs();
        temp_chiplet->set_max_pin_col_num();
        temp_chiplet->set_max_pin_row_num();
        for (int j = 0; j < temp_chiplet->max_pin_row_num; j++)
        {
            for (int k = 0; k < temp_chiplet->max_pin_col_num; k++)
            {
                Coord *coord_to_output = temp_chiplet->get_n_erase_prior_coord();
                string netname_to_output = find_coord_n_signal_in_net_mr3(coord_to_output, vec_net);
                outfile << netname_to_output;
                if (k != temp_chiplet->max_pin_col_num - 1)
                    outfile << ",";
            }
            outfile << endl;
        }
        cout << "[output file] " << filename_output << endl;
        outfile.close();
    }
}

void gen_used_pin_mr3(vector<Chiplet *> &vec_chiplet, vector<Net *> vec_net, IO_parse *IOs_parse)
{
    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        vec_chiplet[i]->deep_cp_used_pinLocs_to_mr3();
    }

    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //cout << count++ << endl;
        Net *temp_net = vec_net[i];
        vector<int> vec_idx_chiplet;
        //extract idx of chiplets need to be dealt
        for (size_t j = 0; j < temp_net->pinnames_structure.size(); j++)
        {
            Pin *temp_pin = temp_net->pinnames_structure[j];
            vec_idx_chiplet.push_back(temp_pin->idx_chiplet);
        }

        //get used pins and distribute them to used_pin vector
        if (vec_idx_chiplet.size() == 2) //deal with dummy_2pn
        {
            //deal with dummy_2pn
            vector<Coord *> *vec_coord1, *vec_coord2;

            //call vec_coord1 and vec_coord2  by ptr
            int idx1 = vec_idx_chiplet[0];
            int idx2 = vec_idx_chiplet[1];

            bool is_IO_vec_coord1 = false;
            bool is_IO_vec_coord2 = false;
            //call copy vec_coord1  by ptr
            if (idx1 == (int)(vec_chiplet.size()))
            {
                is_IO_vec_coord1 = true;
                vec_coord1 = &(IOs_parse->placed_pinLocs);
            }
            else
                vec_coord1 = &(vec_chiplet[idx1]->used_pinLocs_mr3);
            //call vec_coord2 by ptr
            if (idx2 == (int)(vec_chiplet.size()))
            {
                is_IO_vec_coord2 = true;
                vec_coord2 = &(IOs_parse->placed_pinLocs);
            }
            else
                vec_coord2 = &(vec_chiplet[idx2]->used_pinLocs_mr3);

            //find min_HPWL pair in double for loop
            double min_HPWL = MAX_NUM;
            int idx_j_min_HPWL = -1, idx_k_min_HPWL = -1;
            for (size_t j = 0; j < (*vec_coord1).size(); j++)
            {
                for (size_t k = 0; k < (*vec_coord2).size(); k++)
                {
                    Coord *coord1 = (*vec_coord1)[j];
                    if (!is_IO_vec_coord1)
                        if (vec_chiplet[idx1]->is_in_used_by_dummy_2pn_pinLocs_mr3(coord1))
                            continue;
                    Coord *coord2 = (*vec_coord2)[k];
                    if (!is_IO_vec_coord2)
                        if (vec_chiplet[idx2]->is_in_used_by_dummy_2pn_pinLocs_mr3(coord2))
                            continue;
                    vector<Coord *> vec_coord;
                    vec_coord.push_back(coord1);
                    vec_coord.push_back(coord2);
                    double HPWL = cal_HPWL(vec_coord);
                    //cout << HPWL << endl;
                    //cout << "try ";
                    if (HPWL < min_HPWL)
                    {
                        // cout << count++ << endl;
                        //cout << HPWL << endl;
                        min_HPWL = HPWL;
                        idx_j_min_HPWL = j;
                        idx_k_min_HPWL = k;
                    }
                    //cout << endl;
                }
            }

            //add selected pinLocs to used_by_dummy_2pn_pinLocs_mr3
            temp_net->add_pinLocs_to_net((*vec_coord1)[idx_j_min_HPWL]);
            temp_net->add_pinLocs_to_net((*vec_coord2)[idx_k_min_HPWL]);
            if (!is_IO_vec_coord1)
                vec_chiplet[idx1]->used_by_dummy_2pn_pinLocs_mr3.push_back((*vec_coord1)[idx_j_min_HPWL]);
            if (!is_IO_vec_coord2)
                vec_chiplet[idx2]->used_by_dummy_2pn_pinLocs_mr3.push_back((*vec_coord2)[idx_j_min_HPWL]);

            // //place selected pin locs (Coord) to each chiplet/IOs_parse data structure respectively
            // if (idx1 == (int)(vec_chiplet.size())) //is IO pin
            // {
            //     IOs_parse->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
            //     (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);
            // }
            // else
            // {
            //     vec_chiplet[idx1]->used_pinLocs.push_back((*vec_coord1)[idx_j_min_HPWL]);
            //     (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);
            // }

            // if (idx2 == (int)(vec_chiplet.size())) //is IO pin
            // {
            //     IOs_parse->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
            //     (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);
            // }
            // else
            // {
            //     vec_chiplet[idx2]->used_pinLocs.push_back((*vec_coord2)[idx_k_min_HPWL]);
            //     (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);
            // }
        }
        else if (vec_idx_chiplet.size() == 3)
        {
            int idx1 = vec_idx_chiplet[0];
            int idx2 = vec_idx_chiplet[1];
            int idx3 = vec_idx_chiplet[2];
            vector<Coord *> *vec_coord1, *vec_coord2, *vec3;
            //pointers
            vec_coord1 = &(vec_chiplet[idx1]->used_pinLocs_mr3);
            //cout << "vec_coord1 size: " << (*vec_coord1).size() << endl;
            vec_coord2 = &(vec_chiplet[idx2]->used_pinLocs_mr3);
            vec3       = &(vec_chiplet[idx3]->used_pinLocs_mr3);

            double min_HPWL = MAX_NUM;
            int idx_j_min_HPWL = -1, idx_k_min_HPWL = -1, idx_l_min_HPWL = -1;
            for (size_t j = 0; j < (*vec_coord1).size(); j++)
            {
                for (size_t k = 0; k < (*vec_coord2).size(); k++)
                {
                    for (size_t l = 0; l < (*vec3).size(); l++)
                    {
                        Coord *coord1 = (*vec_coord1)[j];
                        if (vec_chiplet[idx1]->is_in_used_by_dummy_2pn_pinLocs_mr3(coord1))
                            continue;
                        Coord *coord2 = (*vec_coord2)[k];
                        if (vec_chiplet[idx2]->is_in_used_by_dummy_2pn_pinLocs_mr3(coord2))
                            continue;
                        Coord *coord3 = (*vec3)[l];
                        if (vec_chiplet[idx3]->is_in_used_by_dummy_2pn_pinLocs_mr3(coord3))
                            continue;
                        vector<Coord *> vec_coord;
                        vec_coord.push_back(coord1);
                        vec_coord.push_back(coord2);
                        vec_coord.push_back(coord3);
                        double HPWL = cal_HPWL(vec_coord);
                        //cout << HPWL << endl;
                        if (HPWL < min_HPWL)
                        {
                            //cout << count++ << endl;
                            //cout << HPWL << endl;
                            min_HPWL = HPWL;
                            idx_j_min_HPWL = j;
                            idx_k_min_HPWL = k;
                            idx_l_min_HPWL = l;
                        }
                    }
                }
            }

            //add selected pinLocs to the net
            temp_net->add_pinLocs_to_net_mr3((*vec_coord1)[idx_j_min_HPWL]);
            temp_net->add_pinLocs_to_net_mr3((*vec_coord2)[idx_k_min_HPWL]);
            temp_net->add_pinLocs_to_net_mr3((*vec3)[idx_l_min_HPWL]);

            //place selected pin locs (Coord) to each chiplet's data structure respectively
            //remove 3 pins from the pool
            vec_chiplet[idx1]->used_used_pinLocs_mr3.push_back((*vec_coord1)[idx_j_min_HPWL]);
            (*vec_coord1).erase((*vec_coord1).begin() + idx_j_min_HPWL);

            vec_chiplet[idx2]->used_used_pinLocs_mr3.push_back((*vec_coord2)[idx_k_min_HPWL]);
            (*vec_coord2).erase((*vec_coord2).begin() + idx_k_min_HPWL);

            vec_chiplet[idx3]->used_used_pinLocs_mr3.push_back((*vec3)[idx_l_min_HPWL]);
            (*vec3).erase((*vec3).begin() + idx_l_min_HPWL);
        }
    }
}

void create_nets_3pn_dummy_2pn(vector<Net *> vec_net_origin, vector<Net *> &nets_3pn_dummy_2pn, int max_dummy_2pn)
{
    bool flag_found_1st_3pn = false;
    bool flag_found_2nd_3pn = false;

    for (size_t i = 0; i < vec_net_origin.size(); i++)
    {
        Net *temp_net = vec_net_origin[i];
        if (temp_net->coord_pins.size() == 2)
        {
            if (max_dummy_2pn > 0 && flag_found_2nd_3pn)
            {
                Pin *temp_pin1 = temp_net->pinnames_structure[0];
                Pin *temp_pin2 = temp_net->pinnames_structure[1];
                //int idx_1 = temp_net->pinnames_structure[0]->idx_chiplet;
                //int idx_2 = temp_net->pinnames_structure[1]->idx_chiplet;
                string name_dummy_2pn = "dummy_2pn" + to_string(max_dummy_2pn);
                Net *dummy_2pn = new Net(name_dummy_2pn);
                dummy_2pn->add_new_pinname_structure(temp_pin1);
                dummy_2pn->add_new_pinname_structure(temp_pin2);
                nets_3pn_dummy_2pn.push_back(dummy_2pn);
                max_dummy_2pn--;
            }
            else if (i < 2) // give the 1st two 2pn a high priority
            {
                Pin *temp_pin1 = temp_net->pinnames_structure[0];
                Pin *temp_pin2 = temp_net->pinnames_structure[1];
                //int idx_1 = temp_net->pinnames_structure[0]->idx_chiplet;
                //int idx_2 = temp_net->pinnames_structure[1]->idx_chiplet;
                string name_dummy_2pn = "dummy_2pn" + to_string(max_dummy_2pn);
                Net *dummy_2pn = new Net(name_dummy_2pn);
                dummy_2pn->add_new_pinname_structure(temp_pin1);
                dummy_2pn->add_new_pinname_structure(temp_pin2);
                nets_3pn_dummy_2pn.push_back(dummy_2pn);
                max_dummy_2pn--;
            }
        }
        else if (temp_net->coord_pins.size() == 3)
        {
            if (!flag_found_1st_3pn)
            {
                flag_found_1st_3pn = true;
                nets_3pn_dummy_2pn.push_back(temp_net);
                do
                {
                    temp_net = vec_net_origin[++i];
                } while (temp_net->coord_pins.size() == 2);
                if (temp_net->coord_pins.size() == 3)
                {
                    flag_found_2nd_3pn = true;
                    nets_3pn_dummy_2pn.push_back(temp_net); //2nd 3-pin net
                }
                else
                    cout << "error: wrong pin count!" << endl;
            }
            else
            {
                nets_3pn_dummy_2pn.push_back(temp_net);
            }
        }
    }
}