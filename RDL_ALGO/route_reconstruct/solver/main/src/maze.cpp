#include "tile.h"
#include "parse.h"
#include "io.h"
#include "bp.h"
#include "maze.h"

MazeBlock::MazeBlock(Tile *tile, int layerNum, int h, int i, int j)
{
    this->visited = false;
    if (tile == nullptr)
    {
        this->blocked = true;
        this->parent_tile = nullptr;
        this->is_terminal = false;
        this->dist = -1;
        this->idx_h = h;
        this->idx_i = i;
        this->idx_j = j;
        this->mb_prev = nullptr;
        this->net = nullptr;
    }
    else if (tile->is_terminal && layerNum == 1)
    {
        //cout << "useless function" << endl;
        this->blocked = true;
        this->parent_tile = tile;
        this->dist = -1;
        this->coord = parent_tile->coord;
        this->coord_mid = parent_tile->coord_mid;
        this->is_terminal = parent_tile->is_terminal;
        this->idx_h = h;
        this->idx_i = i;
        this->idx_j = j;
        this->mb_prev = nullptr;
        this->net = nullptr;
    }
    else
    {
        this->blocked = false;
        this->parent_tile = tile;
        this->dist = -1;
        this->coord = parent_tile->coord;
        this->coord_mid = parent_tile->coord_mid;
        this->is_terminal = false;
        this->idx_h = h;
        this->idx_i = i;
        this->idx_j = j;
        this->mb_prev = nullptr;
        this->net = nullptr;
    }
}

MazeBlock::MazeBlock(MazeBlock *mb)
{
    this->blocked = mb->blocked;
    this->parent_tile = mb->parent_tile;
    this->dist = -1;
    this->coord = mb->coord;
    this->coord_mid = mb->coord_mid;
    this->is_terminal = mb->is_terminal;
    this->net = mb->net;
    this->idx_h = mb->idx_h;
    this->idx_i = mb->idx_i;
    this->idx_j = mb->idx_j;
    this->mb_prev = mb->mb_prev;
    this->net = mb->net;
    this->visited = mb->visited;
}

vector<vector<vector<MazeBlock *>>> gen_maze_blocks(vector<vector<Tile *>> vec_tile, int numLayer)
{
    vector<vector<vector<MazeBlock *>>> vec_mazeblock;
    int block_num_horizontal = vec_tile.size() + 2;  //addtional 2 for the boundary
    int block_num_vertical = vec_tile[0].size() + 2; //addtional 2 for the boundary
    for (int h = 0; h < numLayer + 2; h++)
    {
        vector<vector<MazeBlock *>> vec_maze_block_2d;
        for (int i = 0; i < block_num_horizontal; i++)
        {
            vector<MazeBlock *> vec_col_mazeblock;
            for (int j = 0; j < block_num_vertical; j++)
            {
                if (h == 0 || h == numLayer + 1)  // 0、4
                {
                    MazeBlock *new_mazeblock = new MazeBlock(nullptr, h, h, i, j);
                    vec_col_mazeblock.push_back(new_mazeblock);
                }
                else if (i == 0 || i == block_num_horizontal - 1) //左右边界
                {
                    MazeBlock *new_mazeblock = new MazeBlock(nullptr, h, h, i, j);
                    vec_col_mazeblock.push_back(new_mazeblock);
                }
                else if (j == 0 || j == block_num_vertical - 1)
                {
                    MazeBlock *new_mazeblock = new MazeBlock(nullptr, h, h, i, j);
                    vec_col_mazeblock.push_back(new_mazeblock);
                }
                else
                {
                    ///< 非边界tile构建mazeblock
                    Tile *temp_tile = vec_tile[i - 1][j - 1];
                    MazeBlock *new_mazeblock = new MazeBlock(temp_tile, h, h, i, j);
                    vec_col_mazeblock.push_back(new_mazeblock);
                }
            }
            vec_maze_block_2d.push_back(vec_col_mazeblock);
        }
        vec_mazeblock.push_back(vec_maze_block_2d);
    }

    return vec_mazeblock;
}

void print_blocked_mazeblocks(vector<vector<vector<MazeBlock *>>> vec_mazeblock)
{
    for (size_t h = 0; h < vec_mazeblock.size(); h++)
    {
        if (h == 0 || h == vec_mazeblock.size() - 1)
            continue;
        cout << "RDL" << h << ":" << endl;
        for (size_t i = 0; i < vec_mazeblock[h][0].size(); i++)
        {
            for (size_t j = 0; j < vec_mazeblock[h].size(); j++)
            {
                cout << setw(3);
                if (vec_mazeblock[h][j][i]->blocked && vec_mazeblock[h][j][i]->dist == -1 && vec_mazeblock[h][j][i]->net == nullptr)
                    cout << "X";
                else if (vec_mazeblock[h][j][i]->is_terminal)
                    cout << "T";
                else if (vec_mazeblock[h][j][i]->blocked && vec_mazeblock[h][j][i]->dist == -1 && vec_mazeblock[h][j][i]->net != nullptr)
                    cout << "O";
                else if (vec_mazeblock[h][j][i]->blocked && vec_mazeblock[h][j][i]->dist != -1)
                    cout << vec_mazeblock[h][j][i]->dist;
                else
                    cout << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

bool Route::check_is_dest_pin(MazeBlock *last_explored_mb, Coord *coord_des_pin)
{
    return last_explored_mb->coord_mid->is_equal(coord_des_pin);
}

vector<vector<vector<MazeBlock *>>> copy_maze_blocks(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    vector<vector<vector<MazeBlock *>>> new_vec_maze_block;
    for (size_t h = 0; h < vec_maze_block.size(); h++)
    {
        vector<vector<MazeBlock *>> new_vec_maze_block_2d;
        for (size_t i = 0; i < vec_maze_block[0].size(); i++)
        {
            vector<MazeBlock *> new_col_vec_maze_block;
            for (size_t j = 0; j < vec_maze_block[0][0].size(); j++)
            {
                MazeBlock *new_mb = new MazeBlock(vec_maze_block[h][i][j]);
                //new_mb->print_idxhij(); //test
                new_col_vec_maze_block.push_back(new_mb);
            }
            new_vec_maze_block_2d.push_back(new_col_vec_maze_block);
        }
        new_vec_maze_block.push_back(new_vec_maze_block_2d);
    }
    return new_vec_maze_block;
}

// void link_net_to_mazeblock(vector<Net *> vec_net)
// {
//     for (size_t i = 0; i < vec_net.size(); i++)
//     {
//         Net *temp_net = vec_net[i];
//     }
// }

MazeBlock *MazeBlock::find_R(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h][this->idx_i + 1][this->idx_j];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}
MazeBlock *MazeBlock::find_L(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h][this->idx_i - 1][this->idx_j];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}
MazeBlock *MazeBlock::find_U(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h][this->idx_i][this->idx_j + 1];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}
MazeBlock *MazeBlock::find_D(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h][this->idx_i][this->idx_j - 1];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}
MazeBlock *MazeBlock::find_T(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h + 1][this->idx_i][this->idx_j];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}
MazeBlock *MazeBlock::find_B(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    MazeBlock *des_mb = vec_maze_block[this->idx_h - 1][this->idx_i][this->idx_j];
    if (!(des_mb->blocked))
        return des_mb;
    else
        return nullptr;
}

/**
 * @brief 从RDL1中找到坐标为coord_target的pin所在mazeblock格点
 * 
 * @param vec_mazeblocks  整体迷宫的表示
 * @param coord_target  目标pin的坐标
 * @return MazeBlock*   目标pin坐标对应mazeblock格点的表示
 */
MazeBlock *search_pin_coord_in_mazeblocks(vector<vector<vector<MazeBlock *>>> vec_mazeblocks, Coord *coord_target)
{
    vector<vector<MazeBlock *>> vec_mazeblocks_2d = vec_mazeblocks[1]; //RDL1

    for (size_t i = 1; i < vec_mazeblocks_2d.size() - 1; i++)
    {
        for (size_t j = 1; j < vec_mazeblocks_2d[i].size() - 1; j++)
        {
            //cout << i << ", " << j << endl;
            MazeBlock *temp_mb = vec_mazeblocks_2d[i][j];
            if (coord_target->is_equal(temp_mb->coord_mid))
                return temp_mb;
        }
    }
    //cout << "warning: can't find this coord_target in the mazeblocks, this may be an IO pin" << endl;
    return nullptr;
}

/**
 * @brief 在迷宫中找到一个点的邻接节点（左\右\上\下\顶层\底层）， 然后标记为blocked.添加到队列中返回
 * 
 * @param mb_src  一个迷宫节点
 * @param clone_vec_maze_block 一整个迷宫 
 * @return queue<MazeBlock *>  mb_src的所有邻接节点（左\右\上\下\顶层\底层），均被标记为blocked
 */
queue<MazeBlock *> explore_1_to_1(MazeBlock *mb_src, vector<vector<vector<MazeBlock *>>> &clone_vec_maze_block)
{
    //vector<MazeBlock *> route_used_mb;
    //cout << "explore_1_to_1:" << endl;
    if (mb_src->mb_prev != nullptr)
        mb_src->dist = mb_src->mb_prev->dist + 1;
    //bool flag = false;

    queue<MazeBlock *> Q;
    //cout << "find_R " << endl;
    MazeBlock *mb_R = mb_src->find_R(clone_vec_maze_block);
    //cout << "find_L " << endl;
    MazeBlock *mb_L = mb_src->find_L(clone_vec_maze_block);
    //cout << "find_U " << endl;
    MazeBlock *mb_U = mb_src->find_U(clone_vec_maze_block);
    //cout << "find_D " << endl;
    MazeBlock *mb_D = mb_src->find_D(clone_vec_maze_block);
    MazeBlock *mb_T = mb_src->find_T(clone_vec_maze_block);
    MazeBlock *mb_B = mb_src->find_B(clone_vec_maze_block);
    if (mb_R != nullptr)
    {
        mb_R->mark_blocked();
        mb_R->mb_prev = mb_src;
        Q.push(mb_R);
    }
    if (mb_L != nullptr)
    {
        mb_L->mark_blocked();
        mb_L->mb_prev = mb_src;
        Q.push(mb_L);
    }
    if (mb_U != nullptr)
    {
        mb_U->mark_blocked();
        mb_U->mb_prev = mb_src;
        Q.push(mb_U);
    }
    if (mb_D != nullptr)
    {
        mb_D->mark_blocked();
        mb_D->mb_prev = mb_src;
        Q.push(mb_D);
    }
    if (mb_T != nullptr)
    {
        mb_T->mark_blocked();
        mb_T->mb_prev = mb_src;
        Q.push(mb_T);
    }
    if (mb_B != nullptr)
    {
        mb_B->mark_blocked();
        mb_B->mb_prev = mb_src;
        Q.push(mb_B);
    }
    return Q;
}

// deque<MazeBlock *> explore_1_to_many(MazeBlock *mb_src, vector<vector<vector<MazeBlock *>>> &clone_vec_maze_block)
// {
// }
/**
 * @brief 对给定的dq_src_mb和dq_des_mb 源目标集合中寻找最优路径
 * 
 * @param dq_src_mb  S源点集合
 * @param dq_des_mb  T目标点的集合
 * @param vec_maze_block maze迷宫地图 
 * @return queue<MazeBlock *>  最短路径点集合
 */
queue<MazeBlock *> explore_many_to_many(vector<MazeBlock *> dq_src_mb, vector<MazeBlock *> dq_des_mb, vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    queue<MazeBlock *> path_solution_mbs_clone;
    vector<vector<MazeBlock *>> candidates_route_used_mb;

    for (size_t i = 0; i < dq_src_mb.size(); i++)
    {
        //cout << dq_des_mb[0]->idx_h << endl;
        //cout << "for (size_t i = 0; i < dq_src_mb.size(); i++) " << dq_src_mb.size() << " i = " << i << endl;
        vector<MazeBlock *> route_used_mb;
        MazeBlock *explored_mb_pin = nullptr; // the des pin found first
        vector<MazeBlock *> vec_des_mb_clone; // to be compared with explored mbs each time after a 1-1 explore
        MazeBlock *mb_src_original = dq_src_mb[i];
        //cout << "vector<vector<vector<MazeBlock *>>> clone_vec_maze_block = copy_maze_blocks(vec_maze_block);" << endl;
        vector<vector<vector<MazeBlock *>>> clone_vec_maze_block = copy_maze_blocks(vec_maze_block);
        for (size_t j = 0; j < dq_src_mb.size(); j++)
        {
            MazeBlock *temp_src_mb = dq_src_mb[j]->get_mb_in_another_vec_mb(clone_vec_maze_block);
            temp_src_mb->mark_blocked();
        }

        //gen des_mb_clone
        for (size_t j = 0; j < dq_des_mb.size(); j++)
        {
            MazeBlock *mb_des_original = dq_des_mb[j];
            MazeBlock *des_mb_clone = mb_des_original->get_mb_in_another_vec_mb(clone_vec_maze_block);
            vec_des_mb_clone.push_back(des_mb_clone);
            des_mb_clone->blocked = false;
        }

        MazeBlock *starting_mb_clone = mb_src_original->get_mb_in_another_vec_mb(clone_vec_maze_block);
        // if (mb_src_original->dist == -1) //the first pin
        //     starting_mb_clone->dist = 0;
        // else // other expolred mbs and pins
        //     starting_mb_clone->dist = mb_src_original->dist;
        starting_mb_clone->dist = 0;
        starting_mb_clone->mark_blocked();

        queue<MazeBlock *> q_mb_explored;
        q_mb_explored.push(starting_mb_clone);
        bool found_des = false;
        while (!q_mb_explored.empty() && !found_des)
        {
            queue<MazeBlock *> temp_Q; ///< 找到当前节点所有的邻接节点
            temp_Q = explore_1_to_1(q_mb_explored.front(), clone_vec_maze_block);
            //cout << "dd" << endl;

            ///< 一次处理所有邻接节点，如果存在目标接点，那么找到路径，否则push到队列q_mb_explored中，下一次继续寻找
            while (!temp_Q.empty())
            {
                //if found a pin in the des_clone 找到一个就行了， 为什么？
                for (size_t j = 0; j < vec_des_mb_clone.size(); j++)
                {
                    MazeBlock *des_mb_clone = vec_des_mb_clone[j];
                    if (temp_Q.front() == des_mb_clone)
                    {
                        explored_mb_pin = des_mb_clone;
                        temp_Q.front()->dist = temp_Q.front()->mb_prev->dist + 1;
                        found_des = true;
                        break; //for
                    }
                    if (found_des)
                        break; //while
                }

                q_mb_explored.push(temp_Q.front());
                temp_Q.pop();
            }
            q_mb_explored.pop();
        }
        //print_blocked_mazeblocks(clone_vec_maze_block);
        if (!found_des)
        {
            //cout << "error: failed to find feasible routing solution for this src idx \"" << i << "\" in dq_src_mb" << endl;
            continue;
        }
        route_used_mb = explored_mb_pin->back_trace_by_prev_mb(clone_vec_maze_block);
        //cout << "candidates_route_used_mb.push_back(route_used_mb);" << endl;
        candidates_route_used_mb.push_back(route_used_mb);
    }

    if (candidates_route_used_mb.size() == 0) // don't find a solution path
    {
        queue<MazeBlock *> Q_null;
        return Q_null;
    }
    //
    int min_WL_by_mb = MAX_NUM;
    int idx_shortest_path = -1;
    for (size_t j = 0; j < candidates_route_used_mb.size(); j++) ///<从候选结果中找到一个最优路径
    {
        int WL_by_mb = candidates_route_used_mb[j].size();
        //cout << "WL_by_mb: " << WL_by_mb << endl;
        if (WL_by_mb < min_WL_by_mb)
        {
            min_WL_by_mb = WL_by_mb;
            idx_shortest_path = j;
        }
    }
    //cout << "min_WL_by_mb: " << min_WL_by_mb << endl;

    for (size_t j = 0; j < candidates_route_used_mb[idx_shortest_path].size(); j++)
    {
        path_solution_mbs_clone.push(candidates_route_used_mb[idx_shortest_path][j]);
    }

    //transfer to the ptr in original vec_maze
    queue<MazeBlock *> path_solution_mbs_original;
    while (!path_solution_mbs_clone.empty())
    {
        MazeBlock *clone_mb = path_solution_mbs_clone.front();
        MazeBlock *original_mb = clone_mb->get_mb_in_another_vec_mb(vec_maze_block);
        //original_mb->coord_mid->print_self();
        //cout << endl;
        path_solution_mbs_original.push(original_mb);
        path_solution_mbs_clone.pop();
    }
    //cout << "path_solution_mbs_original.size: " << path_solution_mbs_original.size() << endl;
    return path_solution_mbs_original;
}

/**
 * @brief BFS搜索到的路径回溯，保存具体路径
 * 
 * @param vec_maze_block  迷宫整体
 * @return vector<MazeBlock *>  返回路径的节点集合
 */
vector<MazeBlock *> MazeBlock::back_trace_by_prev_mb(vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    //cout << "[back_trace_by_prev_mb]" << endl;
    vector<MazeBlock *> used_mbs;
    used_mbs.push_back(this); ///<先把自己放进路径中

    // this->coord_mid->print_self();
    // cout << endl;

    MazeBlock *temp_cur_mb = this;
    MazeBlock *temp_prev_mb = this->mb_prev;
    do
    {
        used_mbs.push_back(temp_prev_mb);

        // temp_prev_mb->coord_mid->print_self();
        // cout << endl;

        temp_cur_mb = temp_prev_mb;
        temp_prev_mb = temp_cur_mb->mb_prev;
    } while (temp_prev_mb != nullptr);

    return used_mbs;
}

MazeBlock *find_mb_for_BP(Coord *coord_bp, vector<BP *> vec_BP, vector<vector<vector<MazeBlock *>>> vec_maze_block)
{
    for (size_t i = 0; i < vec_BP.size(); i++)
    {
        BP *temp_bp = vec_BP[i];
        Tile *temp_tile = temp_bp->parent_tile;
        Coord *temp_coord_bp = temp_bp->coord;
        if (temp_coord_bp->is_equal(coord_bp))
        {
            //return temp_tile->get_mapped_mb_RDL1(vec_maze_block);
            return temp_tile->get_mapped_mb_RDL3(vec_maze_block); //IO in RDL3
        }
    }

    cout << "error: can't find the same coord in vec_bp" << endl;
    //exit(0);
    return nullptr;
}

vector<MazeBlock *> move_solution_path_to_used_mb(queue<MazeBlock *> solution_path_mb, vector<MazeBlock *> &src_mbs, vector<MazeBlock *> &des_mbs)
{
    vector<MazeBlock *> used_mb;

    while (!solution_path_mb.empty())
    {
        MazeBlock *solution_mb = solution_path_mb.front();
        //solution_mb->coord_mid->print_self();
        //cout << endl;
        vector<MazeBlock *> temp_push_to_src_mbs, temp_remove_from_des_mbs;
        bool flag_src_found = false;
        bool flag_des_found = false;
        int idx_des = -1;

        //if mb is not in src, put it to src_mbs
        for (size_t i = 0; i < src_mbs.size(); i++)
        {
            MazeBlock *temp_src_mb = src_mbs[i];
            if (solution_mb == temp_src_mb)
            {
                //cout << "solution_mb == temp_src_mb ";
                //solution_mb->coord_mid->print_self();
                //cout << endl;
                flag_src_found = true;
                break;
            }
        }

        //if mb of pin is found, place it to a temp_vec and remove it later from des_mbs
        for (size_t i = 0; i < des_mbs.size(); i++)
        {
            MazeBlock *temp_des_mb = des_mbs[i];
            if (solution_mb == temp_des_mb)
            {
                flag_des_found = true;
                idx_des = i;
                break;
            }
        }

        if (!flag_src_found)
        {
            src_mbs.push_back(solution_mb);
        }

        used_mb.push_back(solution_mb);
        // else //to put the only one mb to the used_mb...as starting pin is always in the src in the very beginning
        // {
        //     if (src_mbs.size() == 1)
        //     {
        //         solution_mb->coord_mid->print_self();
        //         cout << endl;
        //         used_mb.push_back(solution_mb);
        //     }
        // }

        if (flag_des_found)
        {
            src_mbs.push_back(solution_mb);
            des_mbs.erase(des_mbs.begin() + idx_des);
        }

        solution_path_mb.pop();
    }
    if (src_mbs.size() == 1)
    {
        used_mb.push_back(src_mbs[0]);
    }
    return used_mb;
}

vector<MazeBlock *> route_net(Net *net, vector<vector<vector<MazeBlock *>>> vec_maze_block, vector<BP *> vec_BP)
{
    //cout << "route_net" << endl;
    vector<MazeBlock *> route_used_mb;
    //if (vec_coord_pin.size() == 2 && !(net->is_IO_connection))

   ///< 对于一条net只有两个pin的解决方案
    if (net->coord_pins.size() == 2)
    {

        vector<vector<vector<MazeBlock *>>> clone_vec_maze_block = copy_maze_blocks(vec_maze_block);

        Coord *coord_pin1 = net->coord_pins[0];
        Coord *coord_pin2 = net->coord_pins[1];
        MazeBlock *mb_src_clone = search_pin_coord_in_mazeblocks(clone_vec_maze_block, coord_pin1);
        MazeBlock *mb_des_clone = search_pin_coord_in_mazeblocks(clone_vec_maze_block, coord_pin2);

        if (mb_src_clone == nullptr)
        {
            mb_src_clone = find_mb_for_BP(coord_pin1, vec_BP, clone_vec_maze_block);
            coord_pin1 = mb_src_clone->coord_mid;
            //cout << endl;
        }
        else if (mb_des_clone == nullptr)
        {
            mb_des_clone = find_mb_for_BP(coord_pin2, vec_BP, clone_vec_maze_block);
            coord_pin2 = mb_src_clone->coord_mid;
        }

        //route_used_mb.push_back(mb_src_clone);
        //route_used_mb.push_back(mb_des_clone);
        mb_des_clone->blocked = false;
        //cout << "hhh" << endl;
        bool found_des = false;
        queue<MazeBlock *> Q;
        Q.push(mb_src_clone);
        mb_src_clone->dist = 0;
        mb_src_clone->mark_blocked();
        while (!Q.empty() && !found_des)
        {
            queue<MazeBlock *> temp_Q;
            temp_Q = explore_1_to_1(Q.front(), clone_vec_maze_block); //找到Q.front所有邻接节点
            
            while (!temp_Q.empty())
            {
                if (temp_Q.front() == mb_des_clone) //< 邻接节点中找到des节点就代表找到路径， 结束BFS
                {
                    // coord_pin1->print_self();
                    // cout << " to ";
                    // coord_pin2->print_self();
                    // cout << " now at: ";
                    // Q.front()->coord_mid->print_self();
                    // cout << "dist: " << Q.front()->dist << endl;

                    // cout << "!found! " << endl;
                    temp_Q.front()->dist = temp_Q.front()->mb_prev->dist + 1;    ///< 统计路径长度
                    found_des = true;   ///< 标记为已找到路径
                    break;
                }
                Q.push(temp_Q.front());
                temp_Q.pop();
            }
            Q.pop();
        }
        if (!found_des)
        {
            cout << "error: failed to find feasible routing solution" << endl;
            vector<MazeBlock *> vec_null;
            return vec_null;
        }
        //print_blocked_mazeblocks(clone_vec_maze_block);
        route_used_mb = mb_des_clone->back_trace_by_prev_mb(clone_vec_maze_block);   ///< 回溯
    }
    //if 3+_pin net

    ///< 对于一条net具有3个pin以上的路径解决方案
    else
    {
        //cout << "net pin count: " << net->coord_pins.size() << endl;
        vector<vector<vector<MazeBlock *>>> clone_vec_maze_block = copy_maze_blocks(vec_maze_block);

        deque<Coord *> dq_des_pin_coord;
        deque<Coord *> dq_src_coord;
        for (size_t i = 0; i < net->coord_pins.size(); i++)
        {
            dq_des_pin_coord.push_back(net->coord_pins[i]);
        }
        vector<MazeBlock *> dq_src_mb;                                                                            //Q2
        MazeBlock *mb_start_pin = search_pin_coord_in_mazeblocks(clone_vec_maze_block, dq_des_pin_coord.front()); //first pin to start with
        dq_src_mb.push_back(mb_start_pin);                                                                        //gen Q2
        // cout << "mb_start_pin coord: ";
        // mb_start_pin->coord_mid->print_self();
        // cout << endl;
        dq_des_pin_coord.pop_front();

        vector<MazeBlock *> dq_des_mb;                       //Q1
        for (size_t i = 0; i < dq_des_pin_coord.size(); i++) //gen Q1
        {
            MazeBlock *temp_mb_des = search_pin_coord_in_mazeblocks(clone_vec_maze_block, dq_des_pin_coord[i]);
            temp_mb_des->blocked = false;
            //cout << "temp_mb_des: ";
            //dq_des_pin_coord[i]->print_self();
            //cout << " - ";
            //temp_mb_des->print_idxhij();
            //cout << endl;
            dq_des_mb.push_back(temp_mb_des);
            //cout << temp_mb_des->idx_h << ", " << temp_mb_des->idx_i << ", " << temp_mb_des->idx_j << endl;
        }

        while (!dq_des_mb.empty()) ///< 寻找路径
        {
            //cout << "dq_src_mb.size(): " << dq_src_mb.size() << " dq_des_mb.size() " << dq_des_mb.size() << endl;
            // for (size_t i = 0; i < dq_des_mb.size(); i++)
            // {
            //     //dq_des_mb[i]->print_idxhij();
            //     //cout << " ";
            // }
            // cout << endl;
            queue<MazeBlock *> solution_path_mb = explore_many_to_many(dq_src_mb, dq_des_mb, clone_vec_maze_block);
            if (solution_path_mb.size() == 0)
            {
                cout << "error: failed to find feasible routing solution" << endl;
                vector<MazeBlock *> vec_null;
                return vec_null;
            }
            //cout << "dq_src_mb.size: " << dq_src_mb.size() << endl;
            vector<MazeBlock *> used_mb = move_solution_path_to_used_mb(solution_path_mb, dq_src_mb, dq_des_mb);
            //cout << "[used_mb list]" << endl;
            for (size_t i = 0; i < used_mb.size(); i++)
            {
                //used_mb[i]->coord_mid->print_self();
                //cout << endl;
                route_used_mb.push_back(used_mb[i]);
            }
        }
    }
    
    return route_used_mb;   ////< 返回路径
}
/**
 * @brief 为3+pins nets 布线的maze算法实现
 * 
 * @param net  当前3-pin net
 * @param vec_maze_block  迷宫
 * @param vec_BP  ?这里没有用到
 * @return vector<MazeBlock *>  net布线路径
 */
vector<MazeBlock *> route_net_mr3(Net *net, vector<vector<vector<MazeBlock *>>> vec_maze_block, vector<BP *> vec_BP)
{
    //cout << "route_net" << endl;
    vector<MazeBlock *> route_used_mb;
    //if (vec_coord_pin.size() == 2 && !(net->is_IO_connection))
    if (net->coord_pins.size() == 2)
    {
        //do nothing
    }
    //if 3+_pin net
    else
    {
        //cout << "net pin count: " << net->coord_pins.size() << endl;
        ///< 49x49x5
        vector<vector<vector<MazeBlock *>>> clone_vec_maze_block = copy_maze_blocks(vec_maze_block);

        ///< 声明两个队列，做BFS时使用
        deque<Coord *> dq_des_pin_coord;
        deque<Coord *> dq_src_coord;
        for (size_t i = 0; i < net->coord_pins_mr3.size(); i++)
        {
            dq_des_pin_coord.push_back(net->coord_pins_mr3[i]);   ///< net 的尾端点pin
        }

        ///< 根据第一个pin的坐标找到图中对应的那个格点，并push到vector dq_src_mb中，作为开始节点
        vector<MazeBlock *> dq_src_mb;                                                                            //Q2
        MazeBlock *mb_start_pin = search_pin_coord_in_mazeblocks(clone_vec_maze_block, dq_des_pin_coord.front()); //first pin to start with
        dq_src_mb.push_back(mb_start_pin);                                                                        //gen Q2
        // cout << "mb_start_pin coord: ";
        // mb_start_pin->coord_mid->print_self();
        // cout << endl;
        dq_des_pin_coord.pop_front(); ///< 将开始节点从目标节点中移除
          

        ///< 将剩余两个pin的坐标对应的格点mazeblock复位block状态，并push到vector dq_des_m中, 作为目标节点
        vector<MazeBlock *> dq_des_mb;                       //Q1
        for (size_t i = 0; i < dq_des_pin_coord.size(); i++) //gen Q1
        {
            MazeBlock *temp_mb_des = search_pin_coord_in_mazeblocks(clone_vec_maze_block, dq_des_pin_coord[i]);
            temp_mb_des->blocked = false;
            //cout << "temp_mb_des: ";
            //dq_des_pin_coord[i]->print_self();
            //cout << " - ";
            //temp_mb_des->print_idxhij();
            //cout << endl;
            dq_des_mb.push_back(temp_mb_des); 
            //cout << temp_mb_des->idx_h << ", " << temp_mb_des->idx_i << ", " << temp_mb_des->idx_j << endl;
        }

        ///< 依次处理源节点到两个目标节点
        while (!dq_des_mb.empty())
        {
            //cout << "dq_src_mb.size(): " << dq_src_mb.size() << " dq_des_mb.size() " << dq_des_mb.size() << endl;
            // for (size_t i = 0; i < dq_des_mb.size(); i++)
            // {
            //     //dq_des_mb[i]->print_idxhij();
            //     //cout << " ";
            // }
            // cout << endl;
            queue<MazeBlock *> solution_path_mb = explore_many_to_many(dq_src_mb, dq_des_mb, clone_vec_maze_block);
            if (solution_path_mb.size() == 0)
            {
                cout << "error: failed to find feasible routing solution" << endl;
                vector<MazeBlock *> vec_null;
                return vec_null;
            }
            //cout << "dq_src_mb.size: " << dq_src_mb.size() << endl;
            vector<MazeBlock *> used_mb = move_solution_path_to_used_mb(solution_path_mb, dq_src_mb, dq_des_mb);
            //cout << "[used_mb list]" << endl;
            for (size_t i = 0; i < used_mb.size(); i++)
            {
                //used_mb[i]->coord_mid->print_self();
                //cout << endl;
                route_used_mb.push_back(used_mb[i]);
            }
        }
    }
    return route_used_mb;
}
/**
 * @brief 更新迷宫的状态， 将3pin net布线占用的各点，标记为block，且给当前path增加附加一个net
 * 
 * @param vec_maze_block  迷宫地图
 * @param used_mb  3pin+ net在迷宫中的路径
 * @param net  当前net, 将net相关属性和路径mazeblock联系起来
 */
void update_vec_maze_block(vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<MazeBlock *> used_mb, Net *net)
{

    for (size_t i = 0; i < used_mb.size(); i++)
    {
        int idx_h = used_mb[i]->idx_h, idx_i = used_mb[i]->idx_i, idx_j = used_mb[i]->idx_j;
        MazeBlock *temp_mb = vec_maze_block[idx_h][idx_i][idx_j];
        temp_mb->mark_blocked();   ///< 将迷宫中走完线的3pin的路径标记为block。
        temp_mb->net = net;
        temp_mb->dist = used_mb[i]->dist;
        net->add_to_used_mbs(temp_mb);   ///< 将路径添加到net中
        Tile *parent_tile = temp_mb->parent_tile;  
        if (idx_h == 1)
            parent_tile->net = net;   ///<? 
    }
}

bool maze_route(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP)
{
    cout << "[maze_route for all nets]" << endl;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //cout << "[Net order] " << i << endl;
        Net *temp_net = vec_net[i];
        vector<Coord *> vec_coord_pin = temp_net->coord_pins;
        vector<MazeBlock *> route_used_mb;
        route_used_mb = route_net(temp_net, vec_maze_block, vec_BP);
        if (route_used_mb.size() == 0)
            return false;
        update_vec_maze_block(vec_maze_block, route_used_mb, temp_net);
    }
    return true;
}

bool maze_route_for_3plus_pins(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP)
{
    cout << "[maze_route for 3+ pin nets]" << endl;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //cout << "[Net order] " << i << endl;
        Net *temp_net = vec_net[i];
        //temp_net.
        if (temp_net->coord_pins.size() < 3)
            continue;
        vector<Coord *> vec_coord_pin = temp_net->coord_pins;
        vector<MazeBlock *> route_used_mb;
        route_used_mb = route_net(temp_net, vec_maze_block, vec_BP);
        if (route_used_mb.size() == 0)
            return false;
        update_vec_maze_block(vec_maze_block, route_used_mb, temp_net);
    }
    return true;
}

bool maze_route_for_3plus_pins_minus1(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP)
{
    cout << "[maze_route for 3+ pin nets minus the last one]" << endl;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //cout << "[Net order] " << i << endl;
        Net *temp_net = vec_net[i];
        //temp_net.
        if (temp_net->coord_pins.size() < 3)
            continue;
        else if (i == vec_net.size() - 1)
        {
            cout << "temp_net " << temp_net->netname << " is not routed." << endl;
            continue;
        }
        vector<Coord *> vec_coord_pin = temp_net->coord_pins;
        vector<MazeBlock *> route_used_mb;
        route_used_mb = route_net(temp_net, vec_maze_block, vec_BP);
        if (route_used_mb.size() == 0)
            return false;
        update_vec_maze_block(vec_maze_block, route_used_mb, temp_net);
    }
    return true;
}

//for 3-pin maze routing v2
/**
 * @brief 使用迷宫算法为3-pin的net做布线
 * 
 * @param vec_net  chiplet package 中所有的 net
 * @param vec_maze_block  基于grid的迷宫，是一个3维的对象，纬度是RDL的层数
 * @param vec_BP   ？
 * @return true 
 * @return false 
 */
bool maze_route_for_3plus_pins_mr3(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP)
{
    cout << "[maze_route for 3+ pin nets by used pins]" << endl;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        //cout << "[Net order] " << i << endl;
        Net *temp_net = vec_net[i];
        //temp_net.
        if (temp_net->coord_pins_mr3.size() < 3)
            continue;
        vector<Coord *> vec_coord_pin = temp_net->coord_pins_mr3;
        vector<MazeBlock *> route_used_mb;
        route_used_mb = route_net_mr3(temp_net, vec_maze_block, vec_BP);  ///< 迷宫算法获取3pins的路径
        if (route_used_mb.size() == 0)
            return false;
        update_vec_maze_block(vec_maze_block, route_used_mb, temp_net);
    }
    return true;
}

void map_IO_to_mazeblock(vector<IO *> vec_IO, vector<vector<vector<MazeBlock *>>> &vec_maze_block)
{
    for (size_t i = 0; i < vec_IO.size(); i++)
    {
        IO *temp_IO = vec_IO[i];
        BP *temp_bp = temp_IO->BPptr;
        Tile *temp_tile = temp_bp->parent_tile;
        //MazeBlock *temp_mb_to_map = temp_tile->get_mapped_mb_RDL1(vec_maze_block);//place all IOs to RDL1
        MazeBlock *temp_mb_to_map = temp_tile->get_mapped_mb_RDL3(vec_maze_block); //place all IOs to RDL3
        temp_mb_to_map->is_terminal = true;
    }
}

MazeBlock *MazeBlock::get_mb_in_another_vec_mb(vector<vector<vector<MazeBlock *>>> other_vec_maze_block)
{
    return other_vec_maze_block[this->idx_h][this->idx_i][this->idx_j];
}

void MazeBlock::print_idxhij()
{
    cout << " (" << this->idx_h << ", " << this->idx_i << ", " << this->idx_j << ") ";
}

bool randomized_maze_route(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> &vec_maze_block, vector<BP *> vec_BP, float &routability)
{
    //cout << "randomized maze_route" << endl;

    random_shuffle(vec_net.begin(), vec_net.end());

    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        vector<Coord *> vec_coord_pin = temp_net->coord_pins;
        vector<MazeBlock *> route_used_mb;
        route_used_mb = route_net(temp_net, vec_maze_block, vec_BP);
        if (route_used_mb.size() == 0)
        {
            float num_net_routed = float(i) + 1.0;
            float total_num_net = float(vec_net.size());
            routability = (num_net_routed) / total_num_net;
            cout << "routability: " << routability;

            return false;
        }
        update_vec_maze_block(vec_maze_block, route_used_mb, temp_net);
    }
    return true;
}

bool MazeBlock::is_neighbor(MazeBlock *mb2, int wire_spacing)
{
    bool x_neighbor = false, y_neighbor = false, h_neighbor = false;
    bool x_horizontal = false, y_horizontal = false;
    if (abs(this->coord_mid->x - mb2->coord_mid->x) == wire_spacing)
    {
        x_neighbor = true;
    }
    if (abs(this->coord_mid->x - mb2->coord_mid->x) == 0)
    {
        x_horizontal = true;
    }
    if (abs(this->coord_mid->y - mb2->coord_mid->y) == 20)
    {
        y_neighbor = true;
    }
    if (abs(this->coord_mid->y - mb2->coord_mid->y) == 0)
    {
        y_horizontal = true;
    }
    if (abs(this->idx_h - mb2->idx_h) <= 1)
    {
        h_neighbor = true;
    }

    if (x_neighbor && y_neighbor)
        return false;
    else if (!x_neighbor && !x_horizontal)
        return false;
    else if (!y_neighbor && !y_horizontal)
        return false;
    else if (!h_neighbor)
        return false;
    else
        return true;
}