#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <map>
// #include <lemon/smart_graph.h>
// #include <lemon/network_simplex.h>
// #include <lemon/cost_scaling.h>
#include <chrono>

#include "parse.h"
#include "node.h"
#include "tile.h"
#include "edge.h"
#include "io.h"
#include "bp.h"
#include "maze.h"
#include "rdlblock.h"
#include "nlohmann/json.hpp"

#define MAX_NUM_INT 9999999

using json = nlohmann::ordered_json;
//using namespace lemon;
using namespace std;
//using namespace std::chrono;

//global var
vector<Chiplet *> chiplets;
int W_FP, H_FP; //floorplane dimension
int W_OB, H_OB; //offset boundary dimension
int x_OB, y_OB;
int Offset_Distance = 30; //10 *3
int IO_pad_size = 50;     //50x50
int IO_pin_size = 10;     //50x50
int wire_spacing = 20;    //wire mid-line spacing
int numRDL = 3;
int cost = 1, via_cost = 10;
int iter_compaction = -1;
double clock_period = 2.5;

IO_parse *IOs_parse = new IO_parse;
vector<Net *> nets;

vector<vector<Tile *>> tiles;
vector<IO *> IOs;
vector<Track_IO *> routing_track_io_pad;
vector<BP *> allocated_bp;
vector<vector<vector<MazeBlock *>>> maze_blocks;
vector<vector<vector<MazeBlock *>>> maze_blocks_randomized;
vector<Edge *> edges;
vector<Node *> nodes;
vector<Path *> paths;

Node *source_node = new Node(nullptr, nodes);
Node *target_node = new Node(nullptr, nodes);
//
bool is_run_maze_route_for_3pins = false;
bool is_output_csv = false;
bool is_read_flow_solution = false;
//

//used from pin selection of 3 pin nets and added with a few dummy two pin nets.
vector<Net *> nets_3pn_dummy_2pn;
int max_num_dummy_2pn = 5;
//

/**
 * @brief 解析json表示的2.5D chiplets design
 *  1. 物理尺寸
 *  2. net 网表， 表示signal之间的链接信息
 *  3. pin物理位置（空pin，只有物理位置，没有signal表示）
 *  4. 构建外部IO pad阵列
 * @param filename  
 */
void parsing_from_json(char *filename)
{
    ifstream inFile(filename);
    json myJson;
    inFile >> myJson;
    //parse floorplan_size
    W_FP = myJson["FP_Size"][0];
    H_FP = myJson["FP_Size"][1];

    //parse Chiplet names
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        string new_chiplet_name = myJson["Chiplets"][i]["Name"];
        Chiplet *new_chiplet = new Chiplet(new_chiplet_name);
        chiplets.push_back(new_chiplet);
    }
    //parse Chiplet dimensions
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        if (myJson["Chiplets"][i]["Name"] != chiplets[i]->name)
        {
            cout << "failed to match names" << endl;
            break;
        }
        int w = myJson["Chiplets"][i]["Size"][0], h = myJson["Chiplets"][i]["Size"][1];
        chiplets[i]->width = w;
        chiplets[i]->height = h;
    }

    //parse chiplet location
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        if (myJson["Chiplets"][i]["Name"] != chiplets[i]->name)
        {
            cout << "failed to match names" << endl;
            break;
        }
        int x = myJson["Chiplets"][i]["Location"][0], y = myJson["Chiplets"][i]["Location"][1];
        chiplets[i]->x = x;
        chiplets[i]->y = y;

        //cout << chiplets[i]->name << " " << x << " " << y << endl;
    }

    //parse pin locations + place pin locations
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        Chiplet *temp_chiplet = chiplets[i];
        //cout << myJson["Chiplets"][i]["Name"] << endl;
        if (myJson["Chiplets"][i]["Name"] != chiplets[i]->name)
        {
            cout << "failed to match names" << endl;
            break;
        }
        for (size_t j = 0; j < myJson["Chiplets"][i]["Pin_Locations"].size(); j++)
        {
            //cout << myJson["Chiplets"][i]["Pin_Locations"][j][0] << ", " << myJson["Chiplets"][i]["Pin_Locations"][j][1] << endl;
            int x = myJson["Chiplets"][i]["Pin_Locations"][j][0];
            int y = myJson["Chiplets"][i]["Pin_Locations"][j][1];
            Coord *unplaced_new_coord = new Coord(x, y);
            temp_chiplet->unplaced_pinLocs.push_back(unplaced_new_coord);

            Coord *placed_new_coord = new Coord(temp_chiplet->x + x, temp_chiplet->y + y);
            temp_chiplet->placed_pinLocs.push_back(placed_new_coord);
        }
    }

    //parse pin(signal) names
    for (size_t i = 0; i < myJson["Chiplets"].size(); i++)
    {
        Chiplet *temp_chiplet = chiplets[i];
        if (myJson["Chiplets"][i]["Name"] != chiplets[i]->name)
        {
            cout << "failed to match names" << endl;
            break;
        }

        for (size_t j = 0; j < myJson["Chiplets"][i]["Pin_Names"].size(); j++)
        {
            string new_pin_name = myJson["Chiplets"][i]["Pin_Names"][j];

            string new_new = chiplets[i]->name + "/" + new_pin_name;
            //cout << new_pin_name << endl;
            //temp_chiplet->signals.push_back(new_pin_name);
            temp_chiplet->signals.push_back(new_new);
        }
    }

    //parse IO_parse names
    for (size_t i = 0; i < myJson["Ext_Ports"].size(); i++)
    {
        //cout << myJson["Ext_Ports"][i] << endl;
        string new_io_name = myJson["Ext_Ports"][i];
        IOs_parse->add_new_io(new_io_name);
        //cout << IOs_parse->IOnames.back() << endl;
    }
    IOs_parse->pad_size = IO_pad_size;
    IOs_parse->pin_size = IO_pin_size;

    //parse netnames and pins
    for (auto &x : myJson["Netlist"].items())
    {

        //cout << "netname: " << x.key() << ", value: " << x.value() << '\n';
        Net *new_net = new Net(x.key());
        new_net->delay = MAX_NUM_INT;
        //std::cout << "new net name:" << x.key() << endl;

        for (size_t i = 0; i < x.value().size(); i++)
        {
            //cout << i << ": " << x.value()[i] << endl;
            //new_net->add_new_pinname(x.value()[i]);

            //class pin
            //bool flag_IO = false;

            for (size_t j = 0; j < IOs_parse->IOnames.size(); j++)
            {
                if (x.value()[i] == IOs_parse->IOnames[j])
                {
                    new_net->is_IO_connection = true;
                    Pin *new_pin = new Pin(x.value()[i], chiplets.size()); //use the last index as IO_parse index
                    new_net->add_new_pinname_structure(new_pin);
                    //flag_IO = true;
                    break;
                }
            }
            // if (flag_IO) //name found in IOs_parse, skip chiplets...causes error
            //     break;
            int idx_chiplet = -1;
            for (size_t j = 0; j < chiplets.size(); j++)
            {
                Chiplet *temp_chiplet = chiplets[j];
                for (size_t k = 0; k < temp_chiplet->signals.size(); k++)
                {
                    if (x.value()[i] == temp_chiplet->signals[k])
                    {
                        idx_chiplet = j;
                        Pin *new_pin = new Pin(x.value()[i], idx_chiplet);
                        new_net->add_new_pinname_structure(new_pin);
                        break;
                    }
                }
                if (idx_chiplet != -1)
                    break;
            }
        }
        nets.push_back(new_net);
    }

    // for (size_t i = 0; i < nets.size(); i++)
    // {
    //     cout << nets[i]->pinnames.size() << "\t" << nets[i]->netname << "\tpins:";
    //     for (size_t j = 0; j < nets[i]->pinnames.size(); j++)
    //     {
    //         cout << nets[i]->pinnames[j] << "\t";
    //     }
    //     cout << endl;
    // }

    //load delay time from "pin-connect" output
    //string filename_delaytime = "../../pin-connect/output/netname_by_descending_delay.out";
    //string filename_delaytime = "../../pin-connect/output/sdc_netlist_with_descending_delay.out";
    //string filename_delaytime = "../../sdc_parser/output/sdc_netlist_with_descending_delay_inputdelay.out";
    string filename_wirebudget = "../../sdc_parser/output/sdc_netlist_with_ascending_delay_wirebudget.out";
    //string filename_delaytime = "../../pin-connect/output/sdc_netname_by_descending_delay.out";
    //ifstream infile("../../pin-connect/output/netname_by_descending_delay.out");
    ifstream infile(filename_wirebudget);

    if (!(infile.is_open()))
    {
        cout << "error: failed to load delay time file " << filename_wirebudget << endl;
        return;
    }

    string temp_netname;
    double temp_delay;
    while (infile >> temp_netname >> temp_delay)
    {
        //cout << temp_netname << " " << temp_delay << endl;
        for (size_t i = 0; i < nets.size(); i++)
        {
            string temp_netname_delaylist = nets[i]->netname;
            if (temp_netname == temp_netname_delaylist)
            {
                nets[i]->delay = temp_delay;
                break;
            }
        }
    }

    //Produce IO_parse locations
    //cout << myJson["Ext_Port_Min_Space"] << endl;
    double Ext_Port_Min_Space = myJson["Ext_Port_Min_Space"];
    double avaible_width = W_FP - 2 * (50 + Ext_Port_Min_Space);    //horizontal space for IO_parse units
    double available_height = H_FP - 2 * (50 + Ext_Port_Min_Space); //vertical space for IO_parse units
    double pad_space = 50 + Ext_Port_Min_Space;                     // space an IO_parse pad occupy include IO_parse min-spacing

    int numIO_horizontal = avaible_width / pad_space;
    int numIO_vertical = available_height / pad_space;

    double unit_spacing_horizontal = avaible_width / numIO_horizontal;
    double unit_spacing_vertical = available_height / numIO_horizontal;

    //create horizontal IO_parse's Coord
    for (int i = 0; i < numIO_horizontal; i++)
    {
        int bases_x = 50 + Ext_Port_Min_Space / 2;
        double x = (double)bases_x + 0.5 * (double)unit_spacing_horizontal + (double)i * (double)unit_spacing_horizontal;
        double lower_y = 0.5 * 50;
        double upper_y = H_FP - 0.5 * 50;
        Coord *new_coord_lower = new Coord(x, lower_y);
        Coord *new_coord_upper = new Coord(x, upper_y);
        IOs_parse->placed_pinLocs.push_back(new_coord_lower);
        IOs_parse->placed_pinLocs.push_back(new_coord_upper);
    }

    //create vertical IO_parse's Coord
    for (int i = 0; i < numIO_vertical; i++)
    {
        int bases_y = 50 + Ext_Port_Min_Space / 2;
        double y = (double)bases_y + 0.5 * (double)unit_spacing_vertical + (double)i * (double)unit_spacing_vertical;
        double left_x = 0.5 * 50;
        double right_x = W_FP - 0.5 * 50;
        Coord *new_coord_left = new Coord(left_x, y);
        Coord *new_coord_right = new Coord(right_x, y);
        IOs_parse->placed_pinLocs.push_back(new_coord_left);
        IOs_parse->placed_pinLocs.push_back(new_coord_right);
    }
}

int main(int argc, char **argv)
{

    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-vc")
        {
            //cout << "[via cost] " << argv[i + 1] << endl;
            via_cost = stoi(argv[i + 1]);
        }
    }

    cout << "[via cost] " << via_cost << endl;

    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-rs")
        {
            cout << "[read network flow solution] "
                 << "ON" << endl;
            is_read_flow_solution = true;
        }
    }

    if (is_read_flow_solution)
    {
        for (int i = 0; i < argc; i++)
        {
            if (string(argv[i]) == "-it")
            {
                cout << "[max compaction iteration num] " << argv[i + 1] << endl;
                iter_compaction = stoi(argv[i + 1]);
            }
        }
    }

    ///< 获取chiplet design的物理以及连接信息
    parsing_from_json(argv[1]);
    
    ///< 根据sdcparser得到net timing delay 排序
    cout << "[sort_net_by_ascending_wirebudget]" << endl;
    sort_net_by_ascending_wirebudget(nets, clock_period);

    ///< 为net分配pins
    gen_used_pin(chiplets, nets, IOs_parse);

    //for maze routing 3-pin nets
    create_nets_3pn_dummy_2pn(nets, nets_3pn_dummy_2pn, max_num_dummy_2pn);


    //gen_used_pin_mr3(chiplets, nets, IOs_parse);
    gen_used_pin_mr3(chiplets, nets_3pn_dummy_2pn, IOs_parse);
    
    //构建网格单元
    W_OB = cal_offset_boundary_width(W_FP, Offset_Distance, IO_pad_size);
    H_OB = cal_offset_boundary_width(H_FP, Offset_Distance, IO_pad_size);
    x_OB = Offset_Distance + IO_pad_size;
    y_OB = Offset_Distance + IO_pad_size;
    tiles = gen_tile_vector(x_OB, y_OB, W_OB, H_OB, wire_spacing, chiplets, nodes, edges, W_OB / wire_spacing, H_OB / wire_spacing);

    IOs = gen_IOs(IOs_parse, W_FP, H_FP);
    routing_track_io_pad = gen_routing_track_for_io_pad(x_OB, y_OB, W_OB, H_OB, W_FP, H_FP, IO_pad_size);
    print_io_tracks(routing_track_io_pad);

    IOs_parse->gen_available_bp(tiles);
    route_to_bp(IOs, routing_track_io_pad, IOs_parse, x_OB, y_OB, W_FP, H_FP, allocated_bp);
    find_parent_tile_for_vec_BP(tiles, allocated_bp);
    convert_coordIO_to_coordBp_for_vec_nets(nets, IOs);

    //Maze routing
    maze_blocks = gen_maze_blocks(tiles, numRDL);

    //Print blocked mazeblock layout
    //print_blocked_mazeblocks(maze_blocks);
    map_IO_to_mazeblock(IOs, maze_blocks);


    //maze_route_for_3plus_pins_minus1(nets, maze_blocks, allocated_bp);
    string filename_output_maze_route = "../../python/maze_route_multipin.txt";
    cout << "[maze routing for 3-pin nets]" << endl;
    if (!is_read_flow_solution)
    {
        // maze_route_for_3plus_pins(nets, maze_blocks, allocated_bp);
        // output_3_pin_nets(nets, wire_spacing, filename_output_maze_route);

        maze_route_for_3plus_pins_mr3(nets, maze_blocks, allocated_bp);
        vector<vector<routePoint>> routePoints;
        output_3_pin_nets_mr3(nets, wire_spacing, filename_output_maze_route, routePoints);
        //print_blocked_mazeblocks(maze_blocks);
    }

    create_bpNode_inTile(tiles, nodes);
    assign_chiplet_to_all_tiles(tiles, chiplets);

    //cout << "[create nodes and edges]" << endl;
    create_internal_edges_for_tiles(tiles, edges, via_cost);
    if (!is_read_flow_solution)
        cout << "#nodes:  " << nodes.size() << endl;
    //cout << "#edge(inter):  " << edges.size() << endl;
    create_extenal_edges_for_tiles(tiles, edges, cost, W_OB / wire_spacing, H_OB / wire_spacing);
    //cout << "#edge(inter+exter):  " << edges.size() << endl;
    ofstream outfile_3_pin_nets_res;
    outfile_3_pin_nets_res.open("res_used_by_3_pin_nets.txt");
    if (!outfile_3_pin_nets_res.is_open())
        cout << "error: res_used_by_3_pin_nets.txt cannot be opened" << endl;

    //for 3-pin nets
    if (!is_read_flow_solution)
        del_res_for_all_tiles_by_nets(nets, maze_blocks, outfile_3_pin_nets_res);

    set_demand_supply(nets, chiplets);

    assign_s_t_to_all_2_pin_tiles(tiles, chiplets, edges, source_node, target_node, 0);
    //block_pin_rdl1_edge(tiles);
    if (!is_read_flow_solution)
        cout << "#edge(inter+exter+s-t):  " << edges.size() << endl;
    if (!is_read_flow_solution)
        output_csv_files(edges, nodes, get_num_2_pin_net(nets), get_demand_supply(chiplets));

    //cout << myJson["FP_Size"][0] << "," << myJson["FP_Size"][1] << endl;
    string filename_pinLocs = "../output/outpin.txt";
    pin_loc_output(filename_pinLocs, chiplets, IOs_parse);
    output_bp_routing(IOs, routing_track_io_pad);

    // //read and parse MFMC solution
    if (is_read_flow_solution)
    {
        //cout << "[read and parse MFMC solution]" << endl;
        vector<pair<int, int>> pairs_nodeID = read_flow_solution();

        //cout << "[generate vectors of vectors of nodeIDs]" << endl;
        vector<vector<int>> vec_nodeID_set = gen_nodeID_sets(pairs_nodeID, nodes);

        //cout << "[generate vectors of vectors of Coord3 by nodeIDs]" << endl;
        vector<vector<Coord3 *>> vec_coord3_set_uncleaned = gen_coord3_sets(vec_nodeID_set, nodes, tiles);

        string output_routing_path_file_uncompact = "../../python/routing_path_2pin_nets_uncompact.txt";
        output_routing_path(vec_coord3_set_uncleaned, allocated_bp, output_routing_path_file_uncompact, W_FP, H_FP, IO_pad_size);

        //vector<vector<vector<RDLBlock *>>> rdlblocks = gen_empty_RDLBlock_vec(W_OB / wire_spacing, H_OB / wire_spacing, numRDL + 2, wire_spacing, tiles);
        // cout << "[gen_empty_RDLBlock_vec]" << endl;
        vector<vector<vector<RDLBlock *>>> rdlblocks = gen_empty_RDLBlock_vec(W_FP / wire_spacing, H_FP / wire_spacing, numRDL + 2, wire_spacing, tiles, W_OB, H_OB);
        //cout << "[parse_RDLBlock_vec]" << endl;
        //paths = parse_RDLBlock_vec(rdlblocks, "../../python/routing_path_2pin_nets.txt", "../../python/maze_route_multipin.txt", IO_pad_size + Offset_Distance, wire_spacing);
        //cout << "[parse_RDLBlock_vec]" << endl;
        paths = parse_RDLBlock_vec(rdlblocks, output_routing_path_file_uncompact, filename_output_maze_route, IO_pad_size + Offset_Distance, wire_spacing);
        int cur_cost = MAX_NUM_INT, prev_cost;
        for (int i = 0; i < iter_compaction; i++)
        {
            prev_cost = cur_cost;

            cur_cost = corner_reduction_union(paths, rdlblocks);
            if (prev_cost == cur_cost)
            {
                cout << "[compaction iteration] " << i + 1 << endl;
                break;
            }
        }
        //cout << "[transfer_paths_to_vec_coord3]" << endl;
        vector<vector<Coord3 *>> vec_coord3_set_uncleaned_2 = transfer_paths_to_vec_coord3(paths);
        gen_vec_coord3_uncleaned_for_paths(paths);

        string output_routing_path_file_compact = "../../python/routing_path_2pin_nets_compact.txt";
        output_routing_path(vec_coord3_set_uncleaned_2, allocated_bp, output_routing_path_file_compact, W_FP, H_FP, IO_pad_size); // for python
        write_path(paths, allocated_bp, W_FP, H_FP, IO_pad_size);

        vector<Path *> path_2pin;
        vector<Path *> path_multipin;

        divide_paths(paths, path_2pin, path_multipin); //divide paths into 2-pin and multi-pin nets
        sort_2pin_paths_by_ascending_WL(path_2pin);

        //sort_2pin_paths_by_descending_WL(path_2pin);

        cout << "[read_paths_into_nets]" << endl;
        read_paths_into_nets(nets, path_2pin, path_multipin, IOs, chiplets); //assign signals

        cout << "[output_solution_to_json_2]" << endl;
        output_solution_to_json_2(argv[1], nets, IOs);
        cout << "[output_signal_names_as_csv_table_mr3]" << endl;
        //output_signal_names_as_csv_table(nets, chiplets);
        output_signal_names_as_csv_table_mr3(nets, chiplets);
    }
}   