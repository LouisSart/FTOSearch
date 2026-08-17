#include "table.hpp"
#include "fto.hpp"

// CORNERS WITH ORIENTATION
auto corner_index(const CubieFTO& fto){
    return fto.corner_index();
}

auto corners_from_index(const unsigned &c) {
    CubieFTO fto;
    fto.set_corners_from_index(c);
    return fto;
}

void corner_apply(const Move &m, CubieFTO& fto){
    fto.corner_apply(m);
}

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";

// EDGES
auto edge_index(const CubieFTO& fto){
    return fto.ep.index(true);
}

auto edges_from_index(const unsigned &c) {
    CubieFTO fto;
    fto.ep.set_from_index(c, true);
    return fto;
}

void edge_apply(const Move &m, CubieFTO& fto){
    fto.edge_apply(m);
}

static constexpr unsigned CORNER_TABLE_SIZE = 360 * 32;
PruningTable<CORNER_TABLE_SIZE> corner_table;

static constexpr unsigned EDGE_TABLE_SIZE = 239500800;
PruningTable<EDGE_TABLE_SIZE> edge_table;

void generate_corner_table(){    
    corner_table.generate(CubieFTO(), corner_apply, corner_index, corners_from_index, moves);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){    
    edge_table.generate(CubieFTO(), edge_apply, edge_index, edges_from_index, moves, 5, 11);
    edge_table.write(edge_table_path);
    // edge_table.show_distribution();
}