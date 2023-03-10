#include "maze.h"
#include "tile.h"
#include "parse.h"
#include "rdlblock.h"

RDLBlock::RDLBlock(Coord3 *coord, int wire_spacing, int idx_i, int idx_j, int idx_h, Tile *init_tile)
{
    this->coord3 = coord; //要改??
    this->i = idx_i;
    this->j = idx_j;
    this->h = idx_h;
    //this->parent_tile = mb->parent_tile;
    //this->is_terminal = mb->is_terminal;
    this->is_occupied = false;
    this->netname = string(); //default empty string
    this->parent_tile = init_tile;
    this->parent_path = nullptr;
}

vector<vector<vector<RDLBlock *>>> gen_empty_RDLBlock_vec(int max_i, int max_j, int max_h, int wire_spacing, vector<vector<Tile *>> vec_tile, int W_OB, int H_OB)
{

    vector<vector<vector<RDLBlock *>>> vec_plane_rdlblock;
    int min_idx_i_OB = (max_i - W_OB / wire_spacing) / 2;
    int min_idx_j_OB = (max_j - H_OB / wire_spacing) / 2;

    int max_idx_i_OB = min_idx_i_OB + W_OB / wire_spacing - 1;
    int max_idx_j_OB = min_idx_j_OB + H_OB / wire_spacing - 1;

    //cout << "i: " << min_idx_i_OB << " to " << max_idx_i_OB << " j:" << min_idx_j_OB << " to " << max_idx_j_OB << endl;
    for (int i = 0; i < max_i; i++)
    {
        vector<vector<RDLBlock *>> vec_col_rdlblock;
        for (int j = 0; j < max_j; j++)
        {
            vector<RDLBlock *> col_rdlblock;
            for (int h = 0; h < max_h; h++)
            {
                Coord3 *new_coord3;
                Tile *temp_tile;
                if (i >= min_idx_i_OB && i <= max_idx_i_OB && j >= min_idx_j_OB && j <= max_idx_j_OB)
                {
                    temp_tile = vec_tile[i - min_idx_i_OB][j - min_idx_j_OB];
                    //cout << "tile: ";
                    //temp_tile->coord_mid->print_self();
                    //cout << endl;
                    new_coord3 = new Coord3(temp_tile->coord_mid->x, temp_tile->coord_mid->y, h);
                }
                else
                {
                    temp_tile = nullptr;
                    new_coord3 = new Coord3(i * wire_spacing + wire_spacing / 2, j * wire_spacing + wire_spacing / 2, h);
                }

                RDLBlock *new_RDLBlock = new RDLBlock(new_coord3, wire_spacing, i, j, h, temp_tile);
                col_rdlblock.push_back(new_RDLBlock);
            }
            vec_col_rdlblock.push_back(col_rdlblock);
        }
        vec_plane_rdlblock.push_back(vec_col_rdlblock);
    }
    return vec_plane_rdlblock;
}

vector<Path *> parse_RDLBlock_vec(vector<vector<vector<RDLBlock *>>> &vec_RDLBlock, string filename_2_pin_nets_res,
                                  string filename_multi_pin_nets_res, int IOPad_size_and_offdist, int wire_spacing)
{
    ifstream infile_2pin, infile_multipin;
    vector<Path *> vec_path;

    infile_2pin.open(filename_2_pin_nets_res);
    if (!infile_2pin.is_open())
        cout << "error: cannot open filename_2_pin_nets_res" << endl;

    infile_multipin.open(filename_multi_pin_nets_res);
    if (!infile_multipin.is_open())
        cout << "error: cannot open filename_multi_pin_nets_res" << endl;
    string str_line, dummy, is_IO_str, dummy2;
    bool is_IO = false;
    //stringstream ss;
    string str_name, str_x, str_y, str_h, dash;
    Path *prev_path = nullptr;

    //2 pin nets
    while (getline(infile_2pin, str_line))
    {
        //cout << str_line << endl;
        if (str_line.find("WL:") != string::npos)
        {
            stringstream ss2;
            ss2.str(str_line);
            ss2 >> dummy >> dummy >> dummy >> dummy >> dummy >> is_IO_str;
            int is_IO_int = stoi(is_IO_str);
            if (is_IO_int)
                is_IO = true;
            else
                is_IO = false;
            prev_path->is_IO = is_IO;
            continue;
        }
        //cout << str_line << endl;
        stringstream ss;
        ss.str(str_line);
        RDLBlock *prev_turn_block = nullptr;
        Path *new_path;

        while (ss >> str_name >> str_x >> str_y >> str_h)
        {
            // cout << str_name;
            // cout << str_x;
            // cout << str_y;
            // cout << str_h;
            // cout << endl;
            //cout << "hello3" << endl;
            if (str_name.size() != 1) //first RDLB
            {
                //cout << "str_name.size() != 1" << endl;
                str_name.erase(str_name.size() - 1, str_name.size());
                str_name.erase(0, 4);
                //cout << str_name << endl;
                new_path = new Path(str_name);
                new_path->is_multiPin = false;
                vec_path.push_back(new_path);

                str_x.erase(0, 1);
                str_x.erase(str_x.size() - 1, str_x.size());
                str_y.erase(str_y.size() - 1, str_y.size());
                str_h.erase(str_h.size() - 1, str_h.size());
                //cout << str_x << "_" << str_y << "_" << str_h << endl;
                // cout << IOPad_size_and_offdist << endl;
                // int x = stoi(str_x) - IOPad_size_and_offdist;
                // int y = stoi(str_y) - IOPad_size_and_offdist;
                int x = stoi(str_x);
                int y = stoi(str_y);
                int h = stoi(str_h);

                int idx_i = x / wire_spacing;
                int idx_j = y / wire_spacing;
                int idx_h = h;
                //cout << idx_i << ", " << idx_j << ", " << idx_h << endl;
                RDLBlock *temp_RDLB = vec_RDLBlock[idx_i][idx_j][idx_h];
                new_path->rdlb_path.push_back(temp_RDLB);
                temp_RDLB->is_occupied = true;
                temp_RDLB->netname = str_name;
                // cout <<"[first]" <<temp_RDLB->netname<<endl;
                // cout <<str_x<<", " <<str_y <<", "<<idx_h<<endl;
                temp_RDLB->parent_path = new_path;

                //cout << idx_i << ", " << idx_j << ", " << idx_h << endl;
                //cout << temp_RDLB->parent_tile->idx_i << "_" << temp_RDLB->parent_tile->idx_j << endl;

                // cout << "tile x, y: ";
                // temp_RDLB->parent_tile->coord_mid->print_self();
                // cout << endl;
                prev_turn_block = temp_RDLB;
                //cout << "prev_turn_block->i: " << prev_turn_block->i << endl;
            }
            else
            {
                str_x.erase(0, 1);
                str_x.erase(str_x.size() - 1, str_x.size());
                str_y.erase(str_y.size() - 1, str_y.size());
                str_h.erase(str_h.size() - 1, str_h.size());
                //cout << str_x << "_" << str_y << "_" << str_h << endl;

                // int x = stoi(str_x) - IOPad_size_and_offdist;
                // int y = stoi(str_y) - IOPad_size_and_offdist;
                int x = stoi(str_x);
                int y = stoi(str_y);
                int h = stoi(str_h);

                int idx_i = x / wire_spacing;
                int idx_j = y / wire_spacing;
                int idx_h = h;
                //cout << idx_i << ", " << idx_j << ", " << idx_h << endl;
                RDLBlock *temp_RDLB = vec_RDLBlock[idx_i][idx_j][idx_h];
                vector<RDLBlock *> middle_RDLBs = get_middle_plus_des_RDLBs(prev_turn_block, temp_RDLB, vec_RDLBlock);

                while (!middle_RDLBs.empty())
                {
                    // middle_RDLBs.front()->coord3->self_print();
                    // cout <<"---";
                    new_path->rdlb_path.push_back(middle_RDLBs.front());
                    middle_RDLBs.front()->is_occupied = true;
                    middle_RDLBs.front()->netname = str_name;
                    middle_RDLBs.front()->parent_path = new_path;

                    middle_RDLBs.erase(middle_RDLBs.begin());
                }
                //new_path->rdlb_path.push_back(temp_RDLB);
                temp_RDLB->is_occupied = true;
                temp_RDLB->netname = str_name;

                // cout << temp_RDLB->netname<<endl;
                // cout <<str_x<<", " <<str_y <<", "<<idx_h<<endl;
                temp_RDLB->parent_path = new_path;
                //cout << idx_i << ", " << idx_j << ", " << idx_h << endl;
                //cout << temp_RDLB->parent_tile->idx_i << "_" << temp_RDLB->parent_tile->idx_j << endl;

                //cout << "tile x, y: ";
                //temp_RDLB->parent_tile->coord_mid->print_self();
                //cout << endl;
                prev_turn_block = temp_RDLB;
            }
        }
        prev_path = new_path;
    }

    string netname = string();

    // multi-pin nets
    while (getline(infile_multipin, str_line))
    {
        stringstream ss;
        ss.str(str_line);
        Path *new_path = new Path(netname);
        new_path->is_multiPin = true;

        while (ss >> str_x >> str_y >> str_h)
        {
            if (str_x == "net")
            {
                netname = str_y;
                continue;
            }

            str_x.erase(0, 1);
            str_x.erase(str_x.size() - 1, str_x.size());
            str_y.erase(str_y.size() - 1, str_y.size());
            str_h.erase(str_h.size() - 1, str_h.size());

            //int x = stoi(str_x) - IOPad_size_and_offdist;
            //int y = stoi(str_y) - IOPad_size_and_offdist;
            int x = stoi(str_x);
            int y = stoi(str_y);
            int h = stoi(str_h);

            int idx_i = x / wire_spacing;
            int idx_j = y / wire_spacing;
            int idx_h = h;

            RDLBlock *temp_RDLB = vec_RDLBlock[idx_i][idx_j][idx_h];
            new_path->rdlb_path.push_back(temp_RDLB);
            //try fix
            temp_RDLB->is_occupied = true;
            temp_RDLB->netname = netname;
            //
            if (!(ss >> dash))
                break;
        }

        if (new_path->rdlb_path.size() > 0)
            vec_path.push_back(new_path);
    }

    return vec_path;
}

void Path::move_RDLB(RDLBlock *from, RDLBlock *to)
{

    for (size_t i = 0; i < this->rdlb_path.size(); i++)
    {
        if (from == this->rdlb_path[i])
        {
            this->rdlb_path[i] = to;
            from->is_occupied = false;
            from->netname = string();
            to->is_occupied = true;
            to->netname = this->netname;
            break;
        }
    }
}

vector<RDLBlock *> get_middle_plus_des_RDLBs(RDLBlock *from, RDLBlock *to, vector<vector<vector<RDLBlock *>>> vec_RDLBlock)
{
    //cout << "[get_middle_plus_des_RDLBs]" << endl;
    vector<RDLBlock *> vec_RDLB;
    //cout << from->i << ", " << from->j << ", " << from->h << ")from " << endl;
    //cout << to->i << ", " << to->j << ", " << to->h << ")to " << endl;
    if (from->i != to->i)
    {
        if ((from->j != to->j) || (from->h != to->h))
            cout << "error: more than one idx differ(i, j ,h)...i" << endl;
        int size_direction = to->i - from->i;
        int size_abs = abs(size_direction);
        int increamental_unit = size_direction / size_abs;
        int cur_i = from->i;
        while (cur_i != to->i)
        {
            cur_i += increamental_unit;
            vec_RDLB.push_back(vec_RDLBlock[cur_i][from->j][from->h]);
            //vec_RDLBlock[cur_i][from->j][from->h]->coord3->self_print();
            //cout << endl;
        }
    }
    else if (from->j != to->j)
    {
        if ((from->i != to->i) || (from->h != to->h))
            cout << "error: more than one idx differ(i, j ,h)...j" << endl;
        int size_direction = to->j - from->j;
        int size_abs = abs(size_direction);
        int increamental_unit = size_direction / size_abs;
        int cur_j = from->j;
        while (cur_j != to->j)
        {
            cur_j += increamental_unit;
            //cout << "cur_j: " << cur_j << endl;
            vec_RDLB.push_back(vec_RDLBlock[from->i][cur_j][from->h]);
            //vec_RDLBlock[from->i][cur_j][from->h]->coord3->self_print();
            //cout << endl;
        }
    }
    else if (from->h != to->h)
    {
        if ((from->i != to->i) || (from->j != to->j))
            cout << "error: more than one idx differ(i, j ,h)...h" << endl;
        vec_RDLB.push_back(vec_RDLBlock[from->i][from->j][to->h]);
    }
    else
    {
        cout << "error: all idx (i, j ,h) are identical" << endl;
    }
    return vec_RDLB;
}

void decrease_corners(vector<Path *> vec_path, vector<vector<vector<RDLBlock *>>> vec_RDLBlock)
{
    //int prev_total_cost = 0;

    // int count = 0;
    vector<Path *> vec_2_pin_path;

    //filter out 2pin nets
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        Path *temp_path = vec_path[i];
        if (temp_path->netname.find("#") != string::npos)
        {
            vec_2_pin_path.push_back(temp_path);
            // if (temp_path->is_IO)
            //     count++;
        }
    }

    cal_cost_all_path(vec_2_pin_path);
    //prev_total_cost = total_cost;

    for (size_t i = 0; i < vec_2_pin_path.size(); i++)
    {
        cout << "path# " << i << endl;
        bool flag = false;
        bool flag_straight = true;
        bool flag_mid_found = false, flag_lead_found = false;
        Path *temp_path = vec_2_pin_path[i];
        RDLBlock *prev_rdlb = nullptr, *cur_rdlb = nullptr, *next_rdlb = nullptr;
        RDLBlock *rdlb_back = nullptr, *rdlb_mid = nullptr, *rdlb_lead = nullptr;
        //RDLBlock *prev_rdlb_back = nullptr;
        for (size_t j = 0; j < temp_path->rdlb_path.size(); j++)
        {

            cur_rdlb = temp_path->rdlb_path[j];
            if ((j + 1) != temp_path->rdlb_path.size())
                next_rdlb = temp_path->rdlb_path[j + 1];

            if (j == 0)
                rdlb_back = cur_rdlb;

            // if (prev_rdlb == nullptr)
            //     rdlb_back = cur_rdlb;

            if (prev_rdlb != nullptr)
            {
                // cout << "[prev_rdlb != nullptr] ";
                // cur_rdlb->coord3->self_print();
                // next_rdlb->coord3->self_print();
                if (j == temp_path->rdlb_path.size() - 1)
                {
                    if (flag_straight)
                    {
                        temp_path->is_straight = true;
                        cout << "is straight" << endl;
                        break;
                    }
                    //rdlb_back = rdlb_mid;
                    //rdlb_mid = rdlb_lead;
                    rdlb_lead = cur_rdlb;
                    flag_mid_found = true;
                    flag_lead_found = true;
                }
                else
                {
                    flag = cur_rdlb->is_corner(prev_rdlb, next_rdlb);
                    if (flag)
                        flag_straight = false;

                    if (flag)
                    {
                        if (rdlb_mid == nullptr && !flag_mid_found)
                        {
                            rdlb_mid = cur_rdlb;
                            flag_mid_found = true;
                        }
                        // else if (rdlb_mid != nullptr && !flag_mid_found)
                        // {
                        //     flag_mid_found = true;
                        // }
                        else if (!flag_lead_found)
                        {
                            rdlb_lead = cur_rdlb;
                            flag_lead_found = true;
                        }
                        else
                            cout << "error: flag_mid_found/flag_lead_found are not feasible " << endl;
                    }
                }
            }

            //flip and initialize RDLB pointers
            if (flag_mid_found && flag_lead_found)
            {
                rdlb_back->flip_corner(rdlb_mid, rdlb_lead, vec_RDLBlock, temp_path);
                //flag_mid_found = false;
                flag_lead_found = false;

                //prev_rdlb_back = rdlb_mid;
                rdlb_back = rdlb_mid;
                rdlb_mid = rdlb_lead;
                //rdlb_lead = nullptr;
            }

            prev_rdlb = cur_rdlb;
            flag = false;
        }
        cout << "end" << endl;
    }
    cal_cost_all_path(vec_2_pin_path);
}

int cal_cost_all_path(vector<Path *> vec_path)
{
    //cout << "[cal_cost_all_path]" << endl;
    int total_cost = 0;
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        Path *temp_path = vec_path[i];
        total_cost += temp_path->cal_corners();
    }
    //cout << "total_cost: " << total_cost << endl;

    return total_cost;
}

int Path::cal_corners()
{
    RDLBlock *prev_rdlb = nullptr, *cur_rdlb = nullptr, *next_rdlb = nullptr;
    int corner_num = 0;
    bool flag = false;

    for (size_t j = 0; j < this->rdlb_path.size(); j++)
    {

        cur_rdlb = this->rdlb_path[j];
        if ((j + 1) != this->rdlb_path.size())
            next_rdlb = this->rdlb_path[j + 1];
        else
        {
            continue;
        }

        if (prev_rdlb != nullptr)
        {

            flag = cur_rdlb->is_corner(prev_rdlb, next_rdlb);
            if (flag)
                corner_num++;
        }
        prev_rdlb = cur_rdlb;
        flag = false;
    }
    return corner_num;
}

bool RDLBlock::is_corner(RDLBlock *prev_rdlb, RDLBlock *next_rdlb)
{
    if (this->h != prev_rdlb->h || this->h != next_rdlb->h || prev_rdlb->h != next_rdlb->h)
        return false;
    bool is_i_equal_with_prev = false, is_j_equal_with_prev = false;
    bool is_i_equal_with_next = false, is_j_equal_with_next = false;

    if (this->i == prev_rdlb->i)
        is_i_equal_with_prev = true;
    if (this->j == prev_rdlb->j)
        is_j_equal_with_prev = true;
    if (is_i_equal_with_prev && is_j_equal_with_prev)
    {
        cout << "error: prev_rdlb is the same block as the cur_rdlb" << endl;
        return false; //same block
    }
    else if (!is_i_equal_with_prev && !is_j_equal_with_prev)
    {
        cout << "error: prev_rdlb is NOT aligned with the cur_rdlb" << endl;
        return false; //same block
    }

    if (this->i == next_rdlb->i)
        is_i_equal_with_next = true;
    if (this->j == next_rdlb->j)
        is_j_equal_with_next = true;
    if (is_i_equal_with_next && is_j_equal_with_next)
    {
        cout << "error: next_rdlb is the same block as the cur_rdlb" << endl;
        return false; //same block
    }
    else if (!is_i_equal_with_next && !is_j_equal_with_next)
    {
        cout << "error: next_rdlb is NOT aligned with the cur_rdlb" << endl;
        return false; //same block
    }

    if (is_i_equal_with_prev && is_i_equal_with_next)
        return false; //straight line
    else if (is_j_equal_with_prev && is_j_equal_with_next)
        return false; //straight line
    else
        return true;
}

void RDLBlock::flip_corner(RDLBlock *(&rdlb_mid), RDLBlock *rdbl_lead, vector<vector<vector<RDLBlock *>>> vec_RDLBlock, Path *path)
{
    cout << "[flip_corner]" << endl;
    if (this->h != rdlb_mid->h || this->h != rdbl_lead->h || rdlb_mid->h != rdbl_lead->h)
        return;
    cout << "3 coord3: ";
    this->coord3->self_print();
    rdlb_mid->coord3->self_print();
    rdbl_lead->coord3->self_print();
    cout << endl;

    int idx_path_start = -1, idx_path_end = -1;
    int idx_path_mid = -1;
    for (size_t i = 0; i < path->rdlb_path.size(); i++)
    {
        RDLBlock *temp_rdlb = path->rdlb_path[i];
        if (this == temp_rdlb)
            idx_path_start = i;
        else if (rdlb_mid == temp_rdlb)
        {
            idx_path_mid = i;
        }
        else if (rdbl_lead == temp_rdlb)
        {
            idx_path_end = i;
            break;
        }
    }
    vector<RDLBlock *> rdlb_corner;
    for (int i = idx_path_start + 1; i < idx_path_end; i++) //NOT include the first and last rdlb
    {
        rdlb_corner.push_back(path->rdlb_path[i]);
        // path->rdlb_path[i]->coord3->self_print();
        //cout << endl;
    }

    vector<RDLBlock *>::iterator it;
    vector<RDLBlock *> rdlb_new_corner;
    vector<RDLBlock *> segment1;
    if (!rdlb_corner.empty())
    {
        if (this->i != rdlb_mid->i)
        {
            int j_lead = rdbl_lead->j;

            // int signed_distance = rdlb_mid->i - this->i;
            // unsigned int unsigned_dist = abs(rdlb_mid->i - this->i);
            // int increamental_dis = signed_distance / unsigned_dist;
            // int cur_i = this->i + increamental_dis;

            for (it = rdlb_corner.begin(); *it != rdlb_mid; it++)
            {
                RDLBlock *temp_src_rdlb = (*it);
                int idx_i = temp_src_rdlb->i;
                int idx_j = j_lead;
                int idx_h = temp_src_rdlb->h;
                RDLBlock *temp_dest_rdlb = vec_RDLBlock[idx_i][idx_j][idx_h];

                if (temp_dest_rdlb->is_occupied)
                {
                    cout << "intersected while flip" << endl;
                    return;
                }
                else
                {
                    segment1.push_back(temp_dest_rdlb);
                }
            }
        }
        else if (this->j != rdlb_mid->j)
        {
            int i_lead = rdbl_lead->i;
            it = rdlb_corner.begin();
            // (*(it))->coord3->self_print();
            // cout << endl;

            for (it = rdlb_corner.begin(); *it != rdlb_mid; it++)
            {
                RDLBlock *temp_src_rdlb = (*it);

                int idx_i = i_lead;
                int idx_j = temp_src_rdlb->j;
                int idx_h = temp_src_rdlb->h;
                RDLBlock *temp_dest_rdlb = vec_RDLBlock[idx_i][idx_j][idx_h];
                if (temp_dest_rdlb->is_occupied)
                {
                    cout << "intersected while flip" << endl;
                    return;
                }
                else
                {
                    segment1.push_back(temp_dest_rdlb);
                }
            }
        }
        else
        {
            cout << "error: not a valid corner" << endl;
        }
    }

    //mid
    RDLBlock *temp_new_rdlb_mid = nullptr;
    if (this->i == rdlb_mid->i && rdbl_lead->j == rdlb_mid->j)
    {
        int idx_i = rdbl_lead->i;
        int idx_j = this->j;
        int idx_h = this->h;
        temp_new_rdlb_mid = vec_RDLBlock[idx_i][idx_j][idx_h];

        if (temp_new_rdlb_mid->is_occupied)
        {
            cout << "rdlb_mid  intersected while flip" << endl;
            return;
        }
    }
    else if (this->j == rdlb_mid->j && rdbl_lead->i == rdlb_mid->i)
    {
        int idx_i = this->i;
        int idx_j = rdbl_lead->j;
        int idx_h = this->h;
        temp_new_rdlb_mid = vec_RDLBlock[idx_i][idx_j][idx_h];

        if (temp_new_rdlb_mid->is_occupied)
        {
            cout << "rdlb_mid  intersected while flip" << endl;
            return;
        }
    }
    else
    {
        cout << "error: not a valid corner" << endl;
    }

    vector<RDLBlock *> segment2;
    vector<RDLBlock *>::iterator it2;
    if (!rdlb_corner.empty())
    {
        if (rdbl_lead->i != rdlb_mid->i)
        {
            int j_front = this->j;

            for (it2 = it + 1; it2 != rdlb_corner.end(); it2++)
            {
                RDLBlock *temp_src_rdlb = (*it2);
                int idx_i = temp_src_rdlb->i;
                int idx_j = j_front;
                int idx_h = temp_src_rdlb->h;
                RDLBlock *temp_dest_rdlb = vec_RDLBlock[idx_i][idx_j][idx_h];

                if (temp_dest_rdlb->is_occupied)
                {
                    cout << "intersected while flip" << endl;
                    return;
                }
                else
                {
                    segment2.push_back(temp_dest_rdlb);
                }
            }
        }
        else if (rdbl_lead->j != rdlb_mid->j)
        {
            int i_front = this->i;

            for (it2 = it + 1; it2 != rdlb_corner.end(); it2++)
            {
                RDLBlock *temp_src_rdlb = (*it2);
                int idx_i = i_front;
                int idx_j = temp_src_rdlb->j;
                int idx_h = temp_src_rdlb->h;
                RDLBlock *temp_dest_rdlb = vec_RDLBlock[idx_i][idx_j][idx_h];

                if (temp_dest_rdlb->is_occupied)
                {
                    cout << "intersected while flip" << endl;
                    return;
                }
                else
                {
                    segment2.push_back(temp_dest_rdlb);
                }
            }
        }
        else
        {
            cout << "error: not a valid corner" << endl;
        }
    }

    for (size_t i = 0; i < segment2.size(); i++)
    {
        rdlb_new_corner.push_back(segment2[i]);
    }

    rdlb_new_corner.push_back(temp_new_rdlb_mid);

    for (size_t i = 0; i < segment1.size(); i++)
    {
        rdlb_new_corner.push_back(segment1[i]);
    }
    cout << endl;
    for (size_t i = 0; i < rdlb_new_corner.size(); i++)
    {
        rdlb_new_corner[i]->coord3->self_print();
        cout << " ";
    }
    cout << endl;
    int j = 0;
    for (int i = idx_path_start + 1; i < idx_path_mid; i++)
    {
        path->rdlb_path[i]->is_occupied = false;
        path->rdlb_path[i]->parent_path = nullptr;
        string temp_netname = path->rdlb_path[i]->netname;
        path->rdlb_path[i]->netname = string();

        path->rdlb_path[i] = rdlb_new_corner[j];
        path->rdlb_path[i]->is_occupied = true;
        path->rdlb_path[i]->netname = temp_netname;
        path->rdlb_path[i]->parent_path = path;
        j++;
    }

    path->rdlb_path[idx_path_mid] = rdlb_new_corner[j];

    for (int i = idx_path_mid + 1; i < idx_path_end; i++)
    {
        path->rdlb_path[i]->is_occupied = false;
        path->rdlb_path[i]->parent_path = nullptr;
        string temp_netname = path->rdlb_path[i]->netname;
        path->rdlb_path[i]->netname = string();

        path->rdlb_path[i] = rdlb_new_corner[j];
        path->rdlb_path[i]->is_occupied = true;
        path->rdlb_path[i]->netname = temp_netname;
        path->rdlb_path[i]->parent_path = path;
        j++;
    }

    cout << endl;
}

vector<vector<Coord3 *>> transfer_paths_to_vec_coord3(vector<Path *> vec_path)
{
    vector<vector<Coord3 *>> vec_coord3;
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        vector<Coord3 *> col_vec_coord3;
        Path *temp_path = vec_path[i];
        for (size_t j = 0; j < temp_path->rdlb_path.size(); j++)
        {
            Coord3 *temp_coord3 = temp_path->rdlb_path[j]->coord3;
            col_vec_coord3.push_back(temp_coord3);
        }
        vec_coord3.push_back(col_vec_coord3);
    }
    return vec_coord3;
}

void gen_vec_coord3_uncleaned_for_paths(vector<Path *> vec_path)
{
    //vector<vector<Coord3 *>> vec_coord3;
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        //vector<Coord3 *> col_vec_coord3;
        Path *temp_path = vec_path[i];
        for (size_t j = 0; j < temp_path->rdlb_path.size(); j++)
        {
            Coord3 *temp_coord3 = temp_path->rdlb_path[j]->coord3;
            temp_path->coord3_path_uncleaned.push_back(temp_coord3);
            //col_vec_coord3.push_back(temp_coord3);
        }
        //vec_coord3.push_back(col_vec_coord3);
    }
    //return vec_coord3;
}

//union two corners into one
int corner_reduction_union(vector<Path *> vec_path, vector<vector<vector<RDLBlock *>>> vec_RDLBlock)
{
    //int prev_total_cost;
    //int total_cost = 0;
    vector<Path *> vec_2_pin_path;

    //filter out 2pin nets
    for (size_t i = 0; i < vec_path.size(); i++)
    {
        Path *temp_path = vec_path[i];
        if (temp_path->netname.find("#") != string::npos)
        {
            vec_2_pin_path.push_back(temp_path);
            // if (temp_path->is_IO)
            //     count++;
        }
    }
    //cout << " int total_cost = cal_cost_all_path(vec_2_pin_path);" << endl;
    int total_cost = cal_cost_all_path(vec_2_pin_path);
    for (size_t i = 0; i < vec_2_pin_path.size(); i++)
    {
        //cout << "path# " << i << endl;
        RDLBlock *prev_rdlb = nullptr, *cur_rdlb = nullptr, *next_rdlb = nullptr;
        //RDLBlock *rdlb_back = nullptr, *rdlb_mid = nullptr,
        RDLBlock *rdlb_lead = nullptr;
        Path *temp_path = vec_2_pin_path[i];

        pair<RDLBlock *, string> pair_rdlb_back, pair_rdlb_mid, pair_rdlb_lead;
        bool flag_flipped = false;
        bool flag_SL = false;

        for (size_t j = 0; j < temp_path->rdlb_path.size(); j++)
        {
            //cout << "[hello2]" << endl;
            flag_flipped = false;
            flag_SL = false;
            cur_rdlb = temp_path->rdlb_path[j];

            //if (j == 0)
            //rdlb_back = cur_rdlb;
            if (j == temp_path->rdlb_path.size() - 1) //last RDLB in the path
                break;
            if ((j + 1) != temp_path->rdlb_path.size())
                next_rdlb = temp_path->rdlb_path[j + 1];
            // cout << "[hello3]" << endl;
            if (prev_rdlb != nullptr)
            {
                string cur_corner_type = cur_rdlb->get_corner_type(prev_rdlb, next_rdlb);

                // cout << "[cur_corner_type] " << cur_corner_type;
                // cout << " prev_rdlb: ";
                // prev_rdlb->coord3->self_print();
                // cout << " cur_rdlb: ";
                // cur_rdlb->coord3->self_print();
                // cout << " next_rdlb: ";
                // next_rdlb->coord3->self_print();
                // cout << endl;

                if (cur_corner_type == "SL")
                {
                    flag_SL = true;
                }
                else if (cur_corner_type == "ER")
                {
                    cout << "error: ER corner type of prev, cur, next RDLB" << endl;
                }
                else
                {
                    rdlb_lead = cur_rdlb;
                    //pair<RDLBlock *, string> new_pair = make_pair(rdlb_lead, cur_corner_type);
                    //pair_rdlb_lead = new_pair;
                    pair_rdlb_lead.first = rdlb_lead;
                    pair_rdlb_lead.second = cur_corner_type;
                }
                if (!flag_SL && pair_rdlb_back.first != nullptr && cur_corner_type != "VI")
                    if (pair_rdlb_lead.second == pair_rdlb_back.second)
                    {
                        flag_flipped = pair_rdlb_back.first->union_corners(pair_rdlb_mid.first, pair_rdlb_lead.first, vec_RDLBlock);
                    }
            }

            prev_rdlb = cur_rdlb;

            if (!flag_SL)
            {
                pair_rdlb_back = pair_rdlb_mid;
                pair_rdlb_mid = pair_rdlb_lead;
            }
            if (flag_flipped)
            {
                pair<RDLBlock *, string> empty_pair = make_pair(nullptr, "ER");
                pair_rdlb_back = empty_pair;
                pair_rdlb_mid = empty_pair;
                pair_rdlb_lead = empty_pair;
                rdlb_lead = nullptr;
            }
        }
    }

    total_cost = cal_cost_all_path(vec_2_pin_path);
    return total_cost;
}

string RDLBlock::get_corner_type(RDLBlock *prev_rdlb, RDLBlock *next_rdlb)
{

    if (this->h != prev_rdlb->h || this->h != next_rdlb->h || prev_rdlb->h != next_rdlb->h)
        return "VI"; //not the same layer

    bool is_i_equal_with_prev = false, is_j_equal_with_prev = false;
    bool is_i_equal_with_next = false, is_j_equal_with_next = false;

    if (this->i == prev_rdlb->i)
        is_i_equal_with_prev = true;
    if (this->j == prev_rdlb->j)
        is_j_equal_with_prev = true;
    if (is_i_equal_with_prev && is_j_equal_with_prev)
    {
        cout << "error: prev_rdlb is the same block as the cur_rdlb" << endl;
        return "ER"; //same block
    }
    else if (!is_i_equal_with_prev && !is_j_equal_with_prev)
    {
        cout << "error: prev_rdlb is NOT aligned with the cur_rdlb" << endl;
        return "ER"; //not aligned block
    }

    if (this->i == next_rdlb->i)
        is_i_equal_with_next = true;
    if (this->j == next_rdlb->j)
        is_j_equal_with_next = true;
    if (is_i_equal_with_next && is_j_equal_with_next)
    {
        cout << "error: next_rdlb is the same block as the cur_rdlb" << endl;
        return "ER"; //same block
    }
    else if (!is_i_equal_with_next && !is_j_equal_with_next)
    {
        cout << "error: next_rdlb is NOT aligned with the cur_rdlb" << endl;
        return "ER"; //not aligned block
    }

    int case_slash = 0;
    // 1: backslash, 2: forward slash

    if (prev_rdlb->i < next_rdlb->i)
    {
        if (prev_rdlb->j > next_rdlb->j)
            case_slash = 1;
        else if (prev_rdlb->j < next_rdlb->j)
            case_slash = 2;
        else
            //cout << "error: prev_rdlb and next_rdlb are the same j." << endl;
            return "SL"; //straight line
    }
    else if (prev_rdlb->i > next_rdlb->i)
    {
        if (prev_rdlb->j > next_rdlb->j)
            case_slash = 2;
        else if (prev_rdlb->j < next_rdlb->j)
            case_slash = 1;
        else
            //cout << "error: prev_rdlb and next_rdlb are the same j." << endl;
            return "SL"; //straight line
    }
    else
        //cout << "error: prev_rdlb and next_rdlb are having the same i." << endl;
        return "SL"; //straight line

    if (is_i_equal_with_prev && is_i_equal_with_next)
        return "SL"; //straight line
    else if (is_j_equal_with_prev && is_j_equal_with_next)
        return "SL"; //straight line
    else
    {
        switch (case_slash)
        {
        case 0:
            cout << "error: flat prev&next" << endl;
            return "ER";
            break;
        case 1:
            if (this->j < prev_rdlb->j || this->j < next_rdlb->j)
                return "DL";
            else if (this->j > prev_rdlb->j || this->j > next_rdlb->j)
                return "UR";
            break;
        case 2:
            if (this->j < prev_rdlb->j || this->j < next_rdlb->j)
                return "DR";
            else if (this->j > prev_rdlb->j || this->j > next_rdlb->j)
                return "UL";
            break;
        default:
            return "ER";
            break;
        }
        return "ER";
    }
}

void RDLBlock::free_block()
{
    this->is_occupied = false;
    this->parent_path = nullptr;
    this->netname = string();
}

bool RDLBlock::union_corners(RDLBlock *(&rdlb_mid), RDLBlock *rdbl_lead, vector<vector<vector<RDLBlock *>>> vec_RDLBlock)
{
    Path *temp_path = this->parent_path;
    int idx_path_back = parent_path->get_idx_path(this);
    //int idx_path_mid = parent_path->get_idx_path(rdlb_mid);
    int idx_path_lead = parent_path->get_idx_path(rdbl_lead);
    vector<RDLBlock *> vec_flipping_rdlb;
    for (int i = idx_path_back + 1; i < idx_path_lead; i++)
    {
        vec_flipping_rdlb.push_back(temp_path->rdlb_path[i]);
    }
    int idx_i_back = this->i, idx_j_back = this->j;
    int idx_i_lead = rdbl_lead->i, idx_j_lead = rdbl_lead->j;
    int idx_h = this->h;

    int idx_i_mid_orig = rdlb_mid->i, idx_j_mid_orig = rdlb_mid->j;
    int idx_i_mid_new, idx_j_mid_new;

    int idx_case = 0;

    if (idx_i_back == idx_i_mid_orig && idx_j_lead == idx_j_mid_orig) //case2
    {
        idx_i_mid_new = idx_i_lead;
        idx_j_mid_new = idx_j_back;
        idx_case = 2;
    }
    else if (idx_i_lead == idx_i_mid_orig && idx_j_back == idx_j_mid_orig) //case1
    {
        idx_i_mid_new = idx_i_back;
        idx_j_mid_new = idx_j_lead;
        idx_case = 1;
    }
    else
        cout << "error: not a valid combination of back, mid, lead rdlb" << endl;

    vector<RDLBlock *> vec_flipped_rdlb;
    if (idx_case == 2)
    {
        int signed_dist_i = idx_i_mid_new - idx_i_back;
        int signed_dist_j = idx_j_lead - idx_j_mid_new;

        int i_increment = signed_dist_i / abs(signed_dist_i);
        int j_increment = signed_dist_j / abs(signed_dist_j);
        //first segment
        for (int i = idx_i_back + i_increment; i != idx_i_mid_new; i += i_increment)
        {
            RDLBlock *target_rdlb = vec_RDLBlock[i][idx_j_back][idx_h];
            if (target_rdlb->is_occupied)
                return false;
            else
            {
                vec_flipped_rdlb.push_back(target_rdlb);
            }
        }

        //mid block
        RDLBlock *target_rdlb_mid = vec_RDLBlock[idx_i_mid_new][idx_j_mid_new][idx_h];
        if (target_rdlb_mid->is_occupied)
            return false;
        else
            vec_flipped_rdlb.push_back(target_rdlb_mid);

        //second segment
        for (int j = idx_j_mid_new + j_increment; j != idx_j_lead; j += j_increment)
        {
            RDLBlock *target_rdlb = vec_RDLBlock[idx_i_mid_new][j][idx_h];
            if (target_rdlb->is_occupied)
                return false;
            else
            {
                vec_flipped_rdlb.push_back(target_rdlb);
            }
        }
    }
    else if (idx_case == 1)
    {
        int signed_dist_i = idx_i_lead - idx_i_mid_new;
        int signed_dist_j = idx_j_mid_new - idx_j_back;

        int i_increment = signed_dist_i / abs(signed_dist_i);
        int j_increment = signed_dist_j / abs(signed_dist_j);

        //first segment
        for (int j = idx_j_back + j_increment; j != idx_j_mid_new; j += j_increment)
        {
            RDLBlock *target_rdlb = vec_RDLBlock[idx_i_back][j][idx_h];

            if (target_rdlb->is_occupied)
                return false;
            else
            {
                vec_flipped_rdlb.push_back(target_rdlb);
            }
        }

        //mid block
        RDLBlock *target_rdlb_mid = vec_RDLBlock[idx_i_mid_new][idx_j_mid_new][idx_h];
        if (target_rdlb_mid->is_occupied)
            return false;
        else
            vec_flipped_rdlb.push_back(target_rdlb_mid);

        //second segment
        for (int i = idx_i_mid_new + i_increment; i != idx_i_lead; i += i_increment)
        {
            RDLBlock *target_rdlb = vec_RDLBlock[i][idx_j_mid_new][idx_h];
            if (target_rdlb->is_occupied)
                return false;
            else
            {
                vec_flipped_rdlb.push_back(target_rdlb);
            }
        }
    }
    else
    {
        cout << "error: the flipping corner doesn't belong to any of the two cases" << endl;
        return false;
    }
    //replace rdlb in the path and free the unused rdlb
    for (int i = idx_path_back + 1; i != idx_path_lead; i++)
    {
        RDLBlock *temp_rdlb_orig = temp_path->rdlb_path[i];
        temp_rdlb_orig->free_block();
        temp_path->rdlb_path[i] = vec_flipped_rdlb.front();
        vec_flipped_rdlb.front()->occupy_block(temp_path);
        vec_flipped_rdlb.erase(vec_flipped_rdlb.begin());
    }
    return true;
}

int Path::get_idx_path(RDLBlock *rdlb)
{
    for (size_t i = 0; i < this->rdlb_path.size(); i++)
    {
        RDLBlock *temp_rdlb = this->rdlb_path[i];
        if (temp_rdlb == rdlb)
            return i;
    }
    cout << "error: cannot find the specified RDLBlock in this Path" << endl;
    return -1;
}

void RDLBlock::occupy_block(Path *path)
{
    this->is_occupied = true;
    this->parent_path = path;
    this->netname = path->netname;
}

int Path::get_idx_chiplet_for_first_rdl_in_path(vector<Chiplet *> vec_chiplet)
{

    for (size_t j = 0; j < vec_chiplet.size(); j++)
    {
        Chiplet *temp_chiplet = vec_chiplet[j];
        RDLBlock *temp_first_rdlb = this->rdlb_path[0];
        Coord *temp_coord = new Coord(temp_first_rdlb->coord3->x, temp_first_rdlb->coord3->y);
        if (temp_chiplet->is_coord_in_chiplet(temp_coord))
            return j;
    }
    return -1;
}
int Path::get_idx_chiplet_for_last_rdl_in_path(vector<Chiplet *> vec_chiplet)
{
    for (size_t j = 0; j < vec_chiplet.size(); j++)
    {
        Chiplet *temp_chiplet = vec_chiplet[j];
        RDLBlock *temp_last_rdlb = (*(this->rdlb_path.end() - 1));
        Coord *temp_coord = new Coord(temp_last_rdlb->coord3->x, temp_last_rdlb->coord3->y);
        if (temp_chiplet->is_coord_in_chiplet(temp_coord))
            return j;
    }
    return -1;
}