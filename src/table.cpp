#include "table.hpp"
#include "fto.hpp"

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path triangle_table_path = table_dir / "triangles";

static constexpr unsigned CORNER_TABLE_SIZE = Permutation<6, true>::CARD * Orientation<6>::CARD;
PruningTable<CORNER_TABLE_SIZE> corner_table;

static constexpr unsigned EDGE_TABLE_SIZE = Permutation<12, true>::CARD;
PruningTable<EDGE_TABLE_SIZE> edge_table;

static constexpr unsigned TRIANGLE_TABLE_SIZE = Center<12, 4>::CARD;
PruningTable<TRIANGLE_TABLE_SIZE> triangle_table;

void generate_corner_table(){    
    corner_table.generate(CubieFTO(), corner_apply, corner_index, corners_from_index, moves, 3, 4);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){    
    edge_table.generate(CubieFTO(), edge_apply, edge_index, edges_from_index, moves, 5, 11);
    edge_table.write(edge_table_path);
    // edge_table.show_distribution();
}

void generate_triangle_table(){
    triangle_table.generate(CubieFTO(), triangle_apply, tri1_index, tri1_from_index, moves, 3, 7);
    triangle_table.write(triangle_table_path);
    // triangle_table.show_distribution();
}
