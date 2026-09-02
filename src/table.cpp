#include <algorithm>
#include "table.hpp"
#include "coordinate_fto.hpp"

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path triangle_table_path = table_dir / "triangles";

PruningTable<CORNER_CARD> corner_table;
PruningTable<EDGE_CARD> edge_table;
PruningTable<TRIANGLE_CARD> triangle_table;

void generate_corner_table(){  
    print("Generating corner pruning table");  
    corner_table.generate<FTO, true>(corner_index, corners_from_index, moves, 3, 4);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){    
    print("Generating edge pruning table");  
    edge_table.generate<FTO, true>(edge_index, edges_from_index, moves, 5, 11);
    edge_table.write(edge_table_path);
    // edge_table.show_distribution();
}

void generate_triangle_table(){
    print("Generating triangle pruning table");
    triangle_table.generate<FTO, true>(tri1_index, tri1_from_index, moves, 3, 7);
    triangle_table.write(triangle_table_path);
    // triangle_table.show_distribution();
}

void generate_pruning_tables() {
    generate_corner_table();
    generate_edge_table();
    generate_triangle_table();
};

bool load_pruning_tables() {
    if (corner_table.load(corner_table_path)
        && edge_table.load(edge_table_path)
        && triangle_table.load(triangle_table_path)) return true;
    print("Pruning tables missing, generate first");
    return false;
}

// Checkme : remove those since we have a loader for all of thme at once ?
void load_corner_table(){
    assert(fs::exists(corner_table_path));
    corner_table.load(corner_table_path);
}

void load_edge_table(){
    assert(fs::exists(edge_table_path));
    edge_table.load(edge_table_path);
}

void load_triangle_table(){
    assert(fs::exists(triangle_table_path));
    triangle_table.load(triangle_table_path);
}

// CHECKME : we should now use the coordinate version
// of FTO for solving
unsigned estimate(const CubieFTO &fto){
    return std::max({corner_table.estimate(fto.corner_index()),
                    edge_table.estimate(fto.ep.index()),
                    triangle_table.estimate(fto.tri1.index()),
                    triangle_table.estimate(fto.tri2.index())}
        );
};
