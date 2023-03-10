#include "node.h"
#include "edge.h"
#include "maze.h"
#include "tile.h"
#include "parse.h"
#include "bp.h"

void Tile::set_coordULDR()
{
    Coord *new_Ucoord = new Coord(this->x + this->width / 2, this->y + this->height);
    Coord *new_Lcoord = new Coord(this->x, this->y + this->height / 2);
    Coord *new_Dcoord = new Coord(this->x + this->width / 2, this->y);
    Coord *new_Rcoord = new Coord(this->x + this->width, this->y + this->height / 2);

    this->Ucoord = new_Ucoord;
    this->Lcoord = new_Lcoord;
    this->Dcoord = new_Dcoord;
    this->Rcoord = new_Rcoord;
}

bool is_pin(Coord *coord_mid, vector<Chiplet *> vec_chiplet)
{
    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        Chiplet *temp_chiplet = vec_chiplet[i];
        for (size_t j = 0; j < temp_chiplet->used_pinLocs.size(); j++)
        {
            Coord *coord_pin = temp_chiplet->used_pinLocs[j];
            if (coord_mid->is_equal(coord_pin))
                return true;
        }
    }
    return false;
}

Tile::Tile(int x_, int y_, int wire_spacing, vector<Chiplet *> vec_chiplet, int idx_i, int idx_j, vector<Node *> &vec_node, vector<Edge *> &vec_edge, int num_tiles_h, int num_tiles_v)
{
    this->x = x_;
    this->y = y_;
    this->idx_i = idx_i;
    this->idx_j = idx_j;
    this->id_pin_node = -1;
    this->net = nullptr;
    this->idx_chiplet = -1;
    //cout << " (" << this->idx_i << ", " << this->idx_j << " )";

    Coord *new_coord = new Coord(this->x, this->y);
    this->coord = new_coord;
    this->x_mid = x_ + wire_spacing / 2;
    this->y_mid = y_ + wire_spacing / 2;
    Coord *new_coord_mid = new Coord(this->x_mid, this->y_mid);
    this->coord_mid = new_coord_mid;
    this->is_terminal = is_pin(coord_mid, vec_chiplet);
    this->is_bp = false;
    //cout << this->is_terminal << " ";
    this->width = this->height = wire_spacing;
    this->set_coordULDR();

    //generate layer nodes
    this->rdl1_in = new Node(this, vec_node);  ///< 每生成一个节点都往vec_node中添加，并记录当前node的索引
    this->rdl2_in = new Node(this, vec_node);
    this->rdl3_in = new Node(this, vec_node);

    this->rdl1_out = new Node(this, vec_node);
    this->rdl2_out = new Node(this, vec_node);
    this->rdl3_out = new Node(this, vec_node);

    //generate via nodes
    this->via_node_1to2_in = new Node(this, vec_node);
    this->via_node_2to3_in = new Node(this, vec_node);
    this->via_node_1to2_out = new Node(this, vec_node);
    this->via_node_2to3_out = new Node(this, vec_node);

    //generate boundary nodes生成边界node
    if ((idx_i == 0) && (idx_j == 0)) //left down
    {
        ///< in
        ///< 右 和上都分别有节点
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node); 
        ///< 左下为空，无节点
        this->l1_in = nullptr;
        this->l2_in = nullptr;
        this->l3_in = nullptr;
        this->d1_in = nullptr;
        this->d2_in = nullptr;
        this->d3_in = nullptr;

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = nullptr;
        this->l2_out = nullptr;
        this->l3_out = nullptr;
        this->d1_out = nullptr;
        this->d2_out = nullptr;
        this->d3_out = nullptr;
    }
    else if ((idx_i == 0) && (idx_j == (num_tiles_v - 1))) //left up
    {
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = nullptr;
        this->u2_in = nullptr;
        this->u3_in = nullptr;
        this->l1_in = nullptr;
        this->l2_in = nullptr;
        this->l3_in = nullptr;
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = nullptr;
        this->u2_out = nullptr;
        this->u3_out = nullptr;
        this->l1_out = nullptr;
        this->l2_out = nullptr;
        this->l3_out = nullptr;
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
    else if ((idx_i == (num_tiles_h - 1)) && (idx_j == 0)) //right down
    {
        this->r1_in = nullptr;
        this->r2_in = nullptr;
        this->r3_in = nullptr;
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node);
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = nullptr;
        this->d2_in = nullptr;
        this->d3_in = nullptr;

        this->r1_out = nullptr;
        this->r2_out = nullptr;
        this->r3_out = nullptr;
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = nullptr;
        this->d2_out = nullptr;
        this->d3_out = nullptr;
    }
    else if ((idx_i == (num_tiles_h - 1)) && (idx_j == (num_tiles_v - 1))) //right up
    {
        this->r1_in = nullptr;
        this->r2_in = nullptr;
        this->r3_in = nullptr;
        this->u1_in = nullptr;
        this->u2_in = nullptr;
        this->u3_in = nullptr;
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = nullptr;
        this->r2_out = nullptr;
        this->r3_out = nullptr;
        this->u1_out = nullptr;
        this->u2_out = nullptr;
        this->u3_out = nullptr;
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
    else if (idx_i == 0)
    {
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node);
        this->l1_in = nullptr;
        this->l2_in = nullptr;
        this->l3_in = nullptr;
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = nullptr;
        this->l2_out = nullptr;
        this->l3_out = nullptr;
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
    else if (idx_i == (num_tiles_h - 1))
    {
        this->r1_in = nullptr;
        this->r2_in = nullptr;
        this->r3_in = nullptr;
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node);
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = nullptr;
        this->r2_out = nullptr;
        this->r3_out = nullptr;
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
    else if (idx_j == 0)
    {
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node);
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = nullptr;
        this->d2_in = nullptr;
        this->d3_in = nullptr;

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = nullptr;
        this->d2_out = nullptr;
        this->d3_out = nullptr;
    }
    else if (idx_j == (num_tiles_v - 1))
    {
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = nullptr;
        this->u2_in = nullptr;
        this->u3_in = nullptr;
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = nullptr;
        this->u2_out = nullptr;
        this->u3_out = nullptr;
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
    else
    {
        this->r1_in = new Node(this, vec_node);
        this->r2_in = new Node(this, vec_node);
        this->r3_in = new Node(this, vec_node);
        this->u1_in = new Node(this, vec_node);
        this->u2_in = new Node(this, vec_node);
        this->u3_in = new Node(this, vec_node);
        this->l1_in = new Node(this, vec_node);
        this->l2_in = new Node(this, vec_node);
        this->l3_in = new Node(this, vec_node);
        this->d1_in = new Node(this, vec_node);
        this->d2_in = new Node(this, vec_node);
        this->d3_in = new Node(this, vec_node);

        this->r1_out = new Node(this, vec_node);
        this->r2_out = new Node(this, vec_node);
        this->r3_out = new Node(this, vec_node);
        this->u1_out = new Node(this, vec_node);
        this->u2_out = new Node(this, vec_node);
        this->u3_out = new Node(this, vec_node);
        this->l1_out = new Node(this, vec_node);
        this->l2_out = new Node(this, vec_node);
        this->l3_out = new Node(this, vec_node);
        this->d1_out = new Node(this, vec_node);
        this->d2_out = new Node(this, vec_node);
        this->d3_out = new Node(this, vec_node);
    }
}

vector<vector<Tile *>> gen_tile_vector(int x_OB, int y_OB, int W_OB, int H_OB, int wire_spacing, vector<Chiplet *> vec_chiplet,
                                       vector<Node *> &vec_node, vector<Edge *> &vec_edge, int num_tiles_h, int num_tiles_v) 
{
    cout << "gen_tile_vector" << endl;
    vector<vector<Tile *>> vec_double;
    if ((W_OB % wire_spacing != 0) || (H_OB % wire_spacing != 0))
    {
        cout << "error: tile count not an integer" << endl;
    }
    int tile_count_horizontal = W_OB / wire_spacing; ///< 等于 num_tiles_h
    int tile_count_vertical = H_OB / wire_spacing; ///< 等于num_tiles_v
    cout << "cell size:  " << tile_count_horizontal << " x " << tile_count_vertical << endl;
    for (int i = 0; i < tile_count_horizontal; i++)
    {
        vector<Tile *> vec_single;
        for (int j = 0; j < tile_count_vertical; j++)
        {
            Tile *new_tile = new Tile(x_OB + i * wire_spacing, y_OB + j * wire_spacing, wire_spacing, vec_chiplet, i, j, vec_node, vec_edge, num_tiles_h, num_tiles_v);
            if (i == 0)
                new_tile->set_as_Lbd();
            if (j == 0)
                new_tile->set_as_Dbd();
            if (i == tile_count_horizontal - 1)
                new_tile->set_as_Rbd();
            if (j == tile_count_vertical - 1)
                new_tile->set_as_Ubd();
            vec_single.push_back(new_tile); //should initialize also the ULDR Coord
        }
        vec_double.push_back(vec_single);
    }
    //cout << endl;
    return vec_double;
}

void print_tile_coord(vector<vector<Tile *>> vec_tiles)
{
    cout << "(x, y) of tiles" << endl;
    for (size_t i = 0; i < vec_tiles.size(); i++)
    {
        for (size_t j = 0; j < vec_tiles[i].size(); j++)
        {
            cout << "(" << vec_tiles[i][j]->x << ", " << vec_tiles[i][j]->y << ")";
            if (vec_tiles[i][j]->Dbd)
                cout << "D";
            if (vec_tiles[i][j]->Rbd)
                cout << "R";
            if (vec_tiles[i][j]->Ubd)
                cout << "U";
            if (vec_tiles[i][j]->Lbd)
                cout << "L";
            cout << " ";
        }
        cout << endl;
    }
}

//create out node for bp
void attach_tiles_to_bp(vector<vector<Tile *>> vec_tile, vector<Node *> &vec_node)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->is_bp)
            {
                if (temp_tile->Lbd)
                {
                    if (temp_tile->l1_out)
                        cout << "error: this tile is on the OB and shoud not have l1, l2, l3" << endl;
                    // temp_tile->l1_out = new Node(temp_tile, vec_node);
                    // temp_tile->l2_out = new Node(temp_tile, vec_node);
                    // temp_tile->l3_out = new Node(temp_tile, vec_node);
                }
                if (temp_tile->Rbd)
                {
                    if (temp_tile->r1_out)
                        cout << "error: this tile is on the OB and shoud not have r1, r2, r3" << endl;
                    // temp_tile->r1_out = new Node(temp_tile, vec_node);
                    // temp_tile->r2_out = new Node(temp_tile, vec_node);
                    // temp_tile->r3_out = new Node(temp_tile, vec_node);
                }
                if (temp_tile->Ubd)
                {
                    if (temp_tile->u1_out)
                        cout << "error: this tile is on the OB and shoud not have u1, u2, u3" << endl;
                    // temp_tile->u1_out = new Node(temp_tile, vec_node);
                    // temp_tile->u2_out = new Node(temp_tile, vec_node);
                    // temp_tile->u3_out = new Node(temp_tile, vec_node);
                }
                if (temp_tile->Dbd)
                {
                    if (temp_tile->d1_out)
                        cout << "error: this tile is on the OB and shoud not have d1, d2, d3" << endl;
                    // temp_tile->d1_out = new Node(temp_tile, vec_node);
                    // temp_tile->d2_out = new Node(temp_tile, vec_node);
                    // temp_tile->d3_out = new Node(temp_tile, vec_node);
                }
            }
        }
    }
}

void create_bpNode_inTile(vector<vector<Tile *>> vec_tile, vector<Node *> &vec_node)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->is_bp)
            {
                temp_tile->node_bp_out = new Node(temp_tile, vec_node);
                temp_tile->node_bp_in = new Node(temp_tile, vec_node);
            }
        }
    }
}

void Tile::create_internal_edges(vector<Edge *> &vec_edge, int via_cost)
{
    //layer node edge for node capacity modelling
    this->edge_rdl1 = new Edge(this->rdl1_in, this->rdl1_out, 0, vec_edge);
    this->edge_rdl2 = new Edge(this->rdl2_in, this->rdl2_out, 0, vec_edge);
    this->edge_rdl3 = new Edge(this->rdl3_in, this->rdl3_out, 0, vec_edge);

    //via node capacity edge
    this->via_edge_1to2 = new Edge(this->via_node_1to2_in, this->via_node_1to2_out, via_cost, vec_edge);
    this->via_edge_2to3 = new Edge(this->via_node_2to3_in, this->via_node_2to3_out, via_cost, vec_edge);

    //layer node to via node
    this->edge_rdl1_out_to_via1to2_in = new Edge(this->rdl1_out, this->via_node_1to2_in, 0, vec_edge);
    new Edge(this->rdl2_out, this->via_node_1to2_in, 0, vec_edge);

    new Edge(this->rdl2_out, this->via_node_2to3_in, 0, vec_edge);
    new Edge(this->rdl3_out, this->via_node_2to3_in, 0, vec_edge);
    //via node to layer node
    new Edge(this->via_node_1to2_out, this->rdl1_in, 0, vec_edge);
    new Edge(this->via_node_1to2_out, this->rdl2_in, 0, vec_edge);

    new Edge(this->via_node_2to3_out, this->rdl2_in, 0, vec_edge);
    new Edge(this->via_node_2to3_out, this->rdl3_in, 0, vec_edge);

    //zero cost edge..find conditions
    if (!(this->Dbd))
    {
        new Edge(this->d1_in, this->rdl1_in, 0, vec_edge);
        new Edge(this->d2_in, this->rdl2_in, 0, vec_edge);
        new Edge(this->d3_in, this->rdl3_in, 0, vec_edge);
        this->edge_rdl1_out_to_d1_out = new Edge(this->rdl1_out, this->d1_out, 0, vec_edge);
        new Edge(this->rdl2_out, this->d2_out, 0, vec_edge);
        new Edge(this->rdl3_out, this->d3_out, 0, vec_edge);
    }
    if (!(this->Ubd))
    {
        new Edge(this->u1_in, this->rdl1_in, 0, vec_edge);
        new Edge(this->u2_in, this->rdl2_in, 0, vec_edge);
        new Edge(this->u3_in, this->rdl3_in, 0, vec_edge);
        this->edge_rdl1_out_to_u1_out = new Edge(this->rdl1_out, this->u1_out, 0, vec_edge);
        new Edge(this->rdl2_out, this->u2_out, 0, vec_edge);
        new Edge(this->rdl3_out, this->u3_out, 0, vec_edge);
    }
    if (!(this->Rbd))
    {
        new Edge(this->r1_in, this->rdl1_in, 0, vec_edge);
        new Edge(this->r2_in, this->rdl2_in, 0, vec_edge);
        new Edge(this->r3_in, this->rdl3_in, 0, vec_edge);
        this->edge_rdl1_out_to_r1_out = new Edge(this->rdl1_out, this->r1_out, 0, vec_edge);
        new Edge(this->rdl2_out, this->r2_out, 0, vec_edge);
        new Edge(this->rdl3_out, this->r3_out, 0, vec_edge);
    }
    if (!(this->Lbd))
    {
        new Edge(this->l1_in, this->rdl1_in, 0, vec_edge);
        new Edge(this->l2_in, this->rdl2_in, 0, vec_edge);
        new Edge(this->l3_in, this->rdl3_in, 0, vec_edge);
        this->edge_rdl1_out_to_l1_out = new Edge(this->rdl1_out, this->l1_out, 0, vec_edge);
        new Edge(this->rdl2_out, this->l2_out, 0, vec_edge);
        new Edge(this->rdl3_out, this->l3_out, 0, vec_edge);
    }
    if (this->is_bp)
    {
        new Edge(this->rdl1_out, this->node_bp_in, 0, vec_edge);
        new Edge(this->rdl2_out, this->node_bp_in, 0, vec_edge);
        new Edge(this->rdl3_out, this->node_bp_in, 0, vec_edge);
        this->edge_bp = new Edge(this->node_bp_in, this->node_bp_out, 0, vec_edge);
    }
}

void create_internal_edges_for_tiles(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int via_cost)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            //cout << "helo" << endl;
            Tile *temp_tile = vec_tile[i][j];
            temp_tile->create_internal_edges(vec_edge, via_cost);
        }
    }
}

//create edges only on tile's right and up
void Tile::create_external_edges(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int cost, int num_tiles_h, int num_tiles_v)
{
    if (this->idx_i != (num_tiles_h - 1)) //if not the last tile on the right
    {
        Tile *temp_tile_right = vec_tile[this->idx_i + 1][this->idx_j];
        new Edge(this->r1_out, temp_tile_right->l1_in, cost, vec_edge);
        new Edge(this->r2_out, temp_tile_right->l2_in, cost, vec_edge);
        new Edge(this->r3_out, temp_tile_right->l3_in, cost, vec_edge);

        new Edge(temp_tile_right->l1_out, this->r1_in, cost, vec_edge);
        new Edge(temp_tile_right->l2_out, this->r2_in, cost, vec_edge);
        new Edge(temp_tile_right->l3_out, this->r3_in, cost, vec_edge);
    }
    if (this->idx_j != (num_tiles_v - 1)) //if not the last tile on the right
    {
        Tile *temp_tile_up = vec_tile[this->idx_i][this->idx_j + 1];
        new Edge(this->u1_out, temp_tile_up->d1_in, cost, vec_edge);
        new Edge(this->u2_out, temp_tile_up->d2_in, cost, vec_edge);
        new Edge(this->u3_out, temp_tile_up->d3_in, cost, vec_edge);

        new Edge(temp_tile_up->d1_out, this->u1_in, cost, vec_edge);
        new Edge(temp_tile_up->d2_out, this->u2_in, cost, vec_edge);
        new Edge(temp_tile_up->d3_out, this->u3_in, cost, vec_edge);
    }
}

void create_extenal_edges_for_tiles(vector<vector<Tile *>> vec_tile, vector<Edge *> &vec_edge, int cost, int num_tiles_h, int num_tiles_v)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            temp_tile->create_external_edges(vec_tile, vec_edge, cost, num_tiles_h, num_tiles_v);
        }
    }
}

void assign_chiplet_to_all_tiles(vector<vector<Tile *>> vec_tile, vector<Chiplet *> vec_chiplet)
{
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->is_terminal)
            {
                for (size_t k = 0; k < vec_chiplet.size(); k++)
                {
                    Chiplet *temp_chiplet = vec_chiplet[k];
                    if (temp_chiplet->is_in_chiplet(temp_tile))
                    {
                        temp_tile->idx_chiplet = k;
                    }
                }
            }

            if (temp_tile->is_bp) //are IO
            {
                temp_tile->idx_chiplet = vec_chiplet.size();
            }
        }
    }
}

void del_res(vector<MazeBlock *> used_mb, vector<vector<vector<MazeBlock *>>> vec_mbs, ofstream &outfile)
{
    for (size_t i = 0; i < used_mb.size(); i++)
    {
        MazeBlock *temp_mb = used_mb[i];
        int idx_h = temp_mb->idx_h;
        Tile *temp_tile_parent = temp_mb->parent_tile;
        if (idx_h == 1)
        {
            outfile << "(" << temp_tile_parent->x_mid << ", " << temp_tile_parent->y_mid << ", " << idx_h << ") - ";
            temp_tile_parent->edge_rdl1->blockEdge();
            // temp_tile_parent->edge_rdl1_out_to_d1_out->blockEdge();
            // temp_tile_parent->edge_rdl1_out_to_l1_out->blockEdge();
            // temp_tile_parent->edge_rdl1_out_to_r1_out->blockEdge();
            // temp_tile_parent->edge_rdl1_out_to_u1_out->blockEdge();
            // temp_tile_parent->edge_rdl1_out_to_via1to2_in->blockEdge();
            // cout << "deleted res: ";
            // temp_tile_parent->coord_mid->print_self();
            // cout << endl;
        }
        else if (idx_h == 2)
        {
            outfile << "(" << temp_tile_parent->x_mid << ", " << temp_tile_parent->y_mid << ", " << idx_h << ") - ";
            temp_tile_parent->edge_rdl2->blockEdge();
        }
        else if (idx_h == 3)
        {
            outfile << "(" << temp_tile_parent->x_mid << ", " << temp_tile_parent->y_mid << ", " << idx_h << ") - ";
            temp_tile_parent->edge_rdl3->blockEdge();
        }

        MazeBlock *mb_up = temp_mb->get_mb_upstairs(vec_mbs);     //may return nullptr...rdl is topfloor
        MazeBlock *mb_down = temp_mb->get_mb_downstairs(vec_mbs); //may return nullptr...rdl3 is bottom floor
        for (size_t j = 0; j < used_mb.size(); j++)
        {
            if (i == j)
                continue;
            MazeBlock *temp_mb_compare = used_mb[j];
            if (mb_up == temp_mb_compare)
            {
                Tile *temp_mb_up_tile_parent = mb_up->parent_tile;
                if (idx_h == 2)
                {
                    outfile << "(" << temp_mb_up_tile_parent->x_mid << ", " << temp_mb_up_tile_parent->y_mid << ", " << 0 << ") - ";
                    temp_mb_up_tile_parent->via_edge_1to2->blockEdge();
                }
                else if (idx_h == 3)
                {
                    outfile << "(" << temp_mb_up_tile_parent->x_mid << ", " << temp_mb_up_tile_parent->y_mid << ", " << 0 << ") - ";
                    temp_mb_up_tile_parent->via_edge_2to3->blockEdge();
                }
                else
                {
                    cout << "error: mb upstairs is not a valid block, idx_h: " << idx_h << endl;
                }
            }
            if (mb_down == temp_mb_compare)
            {
                Tile *temp_mb_down_tile_parent = mb_down->parent_tile;
                if (idx_h == 1)
                {
                    outfile << "(" << temp_mb_down_tile_parent->x_mid << ", " << temp_mb_down_tile_parent->y_mid << ", " << 0 << ") - ";
                    temp_mb_down_tile_parent->via_edge_1to2->blockEdge();
                }
                else if (idx_h == 2)
                {
                    outfile << "(" << temp_mb_down_tile_parent->x_mid << ", " << temp_mb_down_tile_parent->y_mid << ", " << 0 << ") - ";
                    temp_mb_down_tile_parent->via_edge_2to3->blockEdge();
                }
                else
                {
                    cout << "error: mb downstairs is not a valid block, idx_h: " << idx_h << endl;
                }
            }
        }
    }
}

void del_res_for_all_tiles_by_nets(vector<Net *> vec_net, vector<vector<vector<MazeBlock *>>> vec_mbs, ofstream &outfile)
{
    for (size_t i = 0; i < vec_net.size(); i++)
    {

        Net *temp_net = vec_net[i];
        if (temp_net->coord_pins.size() <= 2)
            continue;
        outfile << "Net#" << i << ": ";
        del_res(temp_net->used_mbs, vec_mbs, outfile);
        outfile << endl;
    }
}

int get_num_2_pin_net(vector<Net *> vec_net)
{
    int num_2_pin_net = 0;
    for (size_t i = 0; i < vec_net.size(); i++)
    {
        Net *temp_net = vec_net[i];
        if (temp_net->coord_pins.size() == 2)
            num_2_pin_net++;
    }
    return num_2_pin_net;
}

void assign_s_t_to_all_2_pin_tiles(vector<vector<Tile *>> vec_tile, vector<Chiplet *> vec_chiplet, vector<Edge *> &vec_edge, Node *s, Node *t, int cost)
{
    //int count_s = 0;
    //int count_t = 0;
    //int count_io = 0;

    vector<int> vec_s;
    vector<int> vec_t;

    for (size_t i = 0; i < vec_chiplet.size(); i++)
    {
        Chiplet *temp_chiplet = vec_chiplet[i];
        vec_s.push_back(temp_chiplet->num_s);
        vec_t.push_back(temp_chiplet->num_t);
    }
    //int count_3_pin_net_pins = 0;
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            // if (temp_tile->idx_chiplet == 2)
            // {
            //     cout << " chiplet idx: " << temp_tile->idx_chiplet << " is_terminal? " << temp_tile->is_terminal << endl;
            // }
            if (temp_tile->is_bp)
            {
                new Edge(temp_tile->node_bp_out, t, 0, vec_edge);
                //count_io++;
                continue;
            }
            if (!temp_tile->is_terminal)
            {
                continue;
            }
            if (temp_tile->net != nullptr) //skip those tiles used by maze routing
            {
                //cout << "netname: " << temp_tile->net->netname;
                //cout << " coord: ";
                //temp_tile->coord_mid->print_self();
                //cout << " chiplet idx: " << temp_tile->idx_chiplet;
                //cout << endl;
                //count_3_pin_net_pins++;
                continue;
            }
            //temp_tile.
            int idx_chiplet = temp_tile->idx_chiplet;
            if (vec_s[idx_chiplet] != 0)
            {
                new Edge(s, temp_tile->rdl1_in, 0, vec_edge);
                vec_s[idx_chiplet]--;
                //count_s++;
            }
            else if (vec_t[idx_chiplet] != 0)
            {
                new Edge(temp_tile->rdl1_out, t, 0, vec_edge);
                vec_t[idx_chiplet]--;
                //count_t++;
            }
        }
    }
    //cout << "count_3_pin_net_pins no s/t connection: " << count_3_pin_net_pins << endl;
    //cout << "count_s, count_t, count_io " << count_s << ", " << count_t << ", " << count_io << endl;
}

int Tile::get_id_pin_node()
{
    if (!this->is_bp || !this->is_terminal)
        return -1;
    if (this->is_bp)
        return this->node_bp_out->idx;
    if (this->is_terminal)
        return this->rdl1_in->idx;
    else
        return -1;
}

int Tile::get_idx_h(Node *node)
{
    if (node == this->l1_in || node == this->l1_out || node == this->r1_in || node == this->r1_out || node == this->u1_in || node == this->u1_out ||
        node == this->d1_in || node == this->d1_out || node == this->rdl1_in || node == this->rdl1_out)
        return 1;
    if (node == this->l2_in || node == this->l2_out || node == this->r2_in || node == this->r2_out || node == this->u2_in || node == this->u2_out ||
        node == this->d2_in || node == this->d2_out || node == this->rdl2_in || node == this->rdl2_out)
        return 2;
    if (node == this->l3_in || node == this->l3_out || node == this->r3_in || node == this->r3_out || node == this->u3_in || node == this->u3_out ||
        node == this->d3_in || node == this->d3_out || node == this->rdl3_in || node == this->rdl3_out)
        return 3;
    else
        return 0;
}

//file reader
vector<pair<int, int>> read_flow_solution()
{
    vector<pair<int, int>> pairs_nodeID;
    ifstream infile;
    infile.open("../input/solution_nodeid.txt");
    if (!infile.is_open())
        cout << "error: cannot open file ../input/solition_nodeid.txt" << endl;
    string dummy;
    int nodeID_src, nodeID_tar;
    infile >> dummy >> dummy;
    while (infile >> nodeID_src >> nodeID_tar)
    {
        //cout << "line: " << nodeID_src << " " << nodeID_tar << endl;
        pair<int, int> new_pair(nodeID_src, nodeID_tar);
        pairs_nodeID.push_back(new_pair);
    }

    return pairs_nodeID;
}

vector<vector<int>> gen_nodeID_sets(vector<pair<int, int>> pairs_nodeID, vector<Node *> vec_node)
{
    vector<vector<int>> vec_nodeID_set;
    while (!pairs_nodeID.empty())
    {
        vector<int> new_nodeID_set;

        pair<int, int> temp_nodeID_pair = pairs_nodeID.back();
        int nodeID_src = temp_nodeID_pair.first;
        int nodeID_tar = temp_nodeID_pair.second;
        //cout << nodeID_src << ", " << nodeID_tar << endl;
        new_nodeID_set.push_back(nodeID_src);
        new_nodeID_set.push_back(nodeID_tar);
        pairs_nodeID.erase(pairs_nodeID.begin() + (int)(pairs_nodeID.size()) - 1);

        while (new_nodeID_set.back() != 1)
        {
            //cout << "new_nodeID_set.back() != 1" << endl;
            for (size_t i = 0; i < pairs_nodeID.size(); i++)
            {
                pair<int, int> temp_pair = pairs_nodeID[i];
                if (temp_pair.first == new_nodeID_set.back())
                {
                    new_nodeID_set.push_back(temp_pair.second);
                    pairs_nodeID.erase(pairs_nodeID.begin() + i);
                    // if (temp_pair.second != 1)
                    // {
                    //     cout << "To target - " << temp_pair.second << " nodename: " << vec_node[temp_pair.second]->parent_tile->get_node_name(vec_node[temp_pair.second]) << " ";
                    //     vec_node[temp_pair.second]->parent_tile->coord_mid->print_self();
                    //     if (vec_node[temp_pair.second]->parent_tile->is_terminal)
                    //         cout << " isPin" << endl;
                    //     else if (vec_node[temp_pair.second]->parent_tile->is_bp)
                    //         cout << " isBP" << endl;
                    //     else
                    //         cout << endl;
                    // }

                    break;
                }
            }
        }

        while (new_nodeID_set.front() != 0)
        {
            //cout << "new_nodeID_set.front() != 0" << endl;
            for (size_t i = 0; i < pairs_nodeID.size(); i++)
            {
                pair<int, int> temp_pair = pairs_nodeID[i];
                if (temp_pair.second == new_nodeID_set.front())
                {
                    vector<int>::iterator it = new_nodeID_set.begin();
                    new_nodeID_set.insert(it, temp_pair.first);
                    pairs_nodeID.erase(pairs_nodeID.begin() + i);
                    // if (temp_pair.second != 0)
                    // {
                    //     cout << "To src - " << temp_pair.second << " nodename: " << vec_node[temp_pair.second]->parent_tile->get_node_name(vec_node[temp_pair.second]) << " ";
                    //     vec_node[temp_pair.second]->parent_tile->coord_mid->print_self();
                    //     if (vec_node[temp_pair.second]->parent_tile->is_terminal)
                    //         cout << " isPin" << endl;
                    //     else if (vec_node[temp_pair.second]->parent_tile->is_bp)
                    //         cout << " isBP" << endl;
                    //     else
                    //         cout << endl;
                    // }
                    break;
                }
            }
        }

        vec_nodeID_set.push_back(new_nodeID_set);
    }
    return vec_nodeID_set;
}

vector<vector<Coord3 *>> gen_coord3_sets(vector<vector<int>> vec_nodeID_set, vector<Node *> vec_node, vector<vector<Tile *>> vec_tile)
{
    vector<vector<Coord3 *>> vec_coord3_sets;
    for (size_t i = 0; i < vec_nodeID_set.size(); i++)
    {
        vector<Coord3 *> vec_coord3;
        vector<int> temp_nodeID_set = vec_nodeID_set[i];
        stack<int> stack_nodeID_no_idx_h;
        stack<Coord3 *> stack_coord3_waiting;

        Coord3 *pre_updated_coord3 = nullptr;
        for (size_t j = 1; j < temp_nodeID_set.size() - 1; j++)
        {
            int nodeID = temp_nodeID_set[j];
            Node *temp_node = vec_node[nodeID];
            Tile *temp_tile = temp_node->parent_tile;

            int idx_h = temp_tile->get_idx_h(temp_node);
            if (idx_h == 0)
            {
                stack_nodeID_no_idx_h.push(nodeID);
            }
            else
            {
                Coord3 *new_coord3 = temp_tile->get_coord3(idx_h);

                if (!(pre_updated_coord3 == nullptr))
                {
                    if (new_coord3->is_equal(pre_updated_coord3))
                    {
                        continue;
                    }
                }
                vec_coord3.push_back(new_coord3);
                stack_coord3_waiting.push(new_coord3);
                pre_updated_coord3 = new_coord3;
            }
        }
        vec_coord3_sets.push_back(vec_coord3);
    }
    return vec_coord3_sets;
}

Coord3 *Tile::get_coord3(int h)
{
    Coord3 *new_coord3 = new Coord3(this->coord_mid->x, this->coord_mid->y, h);
    return new_coord3;
}

string Tile::get_node_name(Node *node)
{
    if (node == this->d1_in)
        return "d1_in";
    else if (node == this->d2_in)
        return "d2_in";
    else if (node == this->d3_in)
        return "d3_in";
    else if (node == this->d1_out)
        return "d1_out";
    else if (node == this->d2_out)
        return "d2_out";
    else if (node == this->d3_out)
        return "d3_out";
    else if (node == this->u1_in)
        return "u1_in";
    else if (node == this->u2_in)
        return "u2_in";
    else if (node == this->u3_in)
        return "u3_in";
    else if (node == this->u1_out)
        return "u1_out";
    else if (node == this->u2_out)
        return "u2_out";
    else if (node == this->u3_out)
        return "u3_out";
    else if (node == this->l1_in)
        return "l1_in";
    else if (node == this->l2_in)
        return "l2_in";
    else if (node == this->l3_in)
        return "l3_in";
    else if (node == this->l1_out)
        return "l1_out";
    else if (node == this->l2_out)
        return "l2_out";
    else if (node == this->l3_out)
        return "l3_out";
    else if (node == this->r1_in)
        return "r1_in";
    else if (node == this->r2_in)
        return "r2_in";
    else if (node == this->r3_in)
        return "r3_in";
    else if (node == this->r1_out)
        return "r1_out";
    else if (node == this->r2_out)
        return "r2_out";
    else if (node == this->r3_out)
        return "r3_out";
    else if (node == this->rdl1_in)
        return "rdl1_in";
    else if (node == this->rdl2_in)
        return "rdl2_in";
    else if (node == this->rdl3_in)
        return "rdl3_in";
    else if (node == this->rdl1_out)
        return "rdl1_out";
    else if (node == this->rdl2_out)
        return "rdl2_out";
    else if (node == this->rdl3_out)
        return "rdl3_out";
    else if (node == this->via_node_1to2_in)
        return "via_node_1to2_in";
    else if (node == this->via_node_2to3_in)
        return "via_node_2to3_in";
    else if (node == this->via_node_1to2_out)
        return "via_node_1to2_out";
    else if (node == this->via_node_2to3_out)
        return "via_node_2to3_out";
    else
        return "error: this node is none of the node in this tile.";
}

void block_pin_rdl1_edge(vector<vector<Tile *>> vec_tile)
{
    int count = 0;
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[0].size(); j++)
        {
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->is_terminal && temp_tile->idx_chiplet == 0)
            {
                temp_tile->edge_rdl1->blockEdge();
                count++;
            }
        }
    }
    //cout << "blocked edge for rdl1: " << count << endl;
}