#include "io.h"
#include "parse.h"
#include "tile.h"
#include "bp.h"

void BP::find_parent_tile(vector<vector<Tile *>> vec_tile)
{
    Coord *temp_this_coord = this->coord;
    for (size_t i = 0; i < vec_tile.size(); i++)
    {
        for (size_t j = 0; j < vec_tile[i].size(); j++)
        {
            // if (i != 0 || i != vec_tile.size() - 1)
            //     continue;
            // if (j != 0 || j != vec_tile[i].size() - 1)
            //     continue;
            Tile *temp_tile = vec_tile[i][j];
            if (temp_tile->Dbd)
                if (temp_this_coord->is_equal(temp_tile->Dcoord))
                {
                    this->parent_tile = temp_tile;
                    temp_tile->is_bp = true;
                    temp_tile->bp = this;
                    temp_tile->bp_side = 'D';
                    return;
                }
            if (temp_tile->Ubd)
                if (temp_this_coord->is_equal(temp_tile->Ucoord))
                {
                    this->parent_tile = temp_tile;
                    temp_tile->is_bp = true;
                    temp_tile->bp = this;
                    temp_tile->bp_side = 'U';
                    return;
                }
            if (temp_tile->Lbd)
                if (temp_this_coord->is_equal(temp_tile->Lcoord))
                {
                    this->parent_tile = temp_tile;
                    temp_tile->is_bp = true;
                    temp_tile->bp = this;
                    temp_tile->bp_side = 'L';
                    return;
                }
            if (temp_tile->Rbd)
                if (temp_this_coord->is_equal(temp_tile->Rcoord))
                {
                    this->parent_tile = temp_tile;
                    temp_tile->is_bp = true;
                    temp_tile->bp = this;
                    temp_tile->bp_side = 'R';
                    return;
                }
        }
    }
}

void find_parent_tile_for_vec_BP(vector<vector<Tile *>> vec_tile, vector<BP *> vec_BP)
{
    for (size_t i = 0; i < vec_BP.size(); i++)
    {
        BP *temp_bp = vec_BP[i];
        temp_bp->find_parent_tile(vec_tile);
    }
}