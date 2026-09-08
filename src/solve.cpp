#include <algorithm>
#include "solve.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path edge_convert_table_path = table_dir / "edge_convert";
fs::path triangle_table_path = table_dir / "triangles";
fs::path triplet_table_path = table_dir / "triplets";

PruningTable<CORNER_CARD> corner_table;
PruningTable<EDGE_CARD> edge_table;
PruningTable<TRIANGLE_CARD> triangle_table;
PruningTable<CORNER_CARD * TRIANGLE_CARD> triplet_table;

void generate_corner_table(){  
    print("Generating corner pruning table");  
    corner_table.generate<FTO, true>(corner_index, corners_from_index, moves, 3, 4);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){  
    print("Generating edge index conversion table");
    generate_edge_convert_table();
    write_edge_convert_table(edge_convert_table_path);
    print("Generating edge pruning table");
    edge_table.generate<FTO, true>(dense_edge_index, edges_from_dense_index, moves, 5, 11);
    edge_table.write(edge_table_path);
    // edge_table.show_distribution();
}

void generate_triangle_table(){
    print("Generating triangle pruning table");
    triangle_table.generate<FTO, true>(tri1_index, tri1_from_index, moves, 3, 7);
    triangle_table.write(triangle_table_path);
    // triangle_table.show_distribution();
}

void generate_triplet_table(){
    print("Generating triplet pruning table");
    triplet_table.generate<FTO, true>(triplet_index, from_triplet_index, moves, 7, 10);
    triplet_table.write(triangle_table_path);
    // triangle_table.show_distribution();
}

void generate_pruning_tables() {
    generate_corner_table();
    generate_edge_table();
    generate_triangle_table();
    generate_triplet_table();
};

bool load_pruning_tables() {
    if (corner_table.load(corner_table_path)
        && edge_table.load(edge_table_path)
        && load_edge_convert_table(edge_convert_table_path)
        && triangle_table.load(triangle_table_path)
        && triplet_table.load(triplet_table_path)) return true;
    print("Pruning tables missing, generate first");
    return false;
}

unsigned estimate(const CubieFTO &fto){
    return std::max({corner_table.estimate(fto.corner_index()),
                    edge_table.estimate(fto.ep.index()),
                    triangle_table.estimate(fto.tri1.index()),
                    triangle_table.estimate(fto.tri2.index())}
        );
};

unsigned estimate(const FTO& fto) {
    return std::max({
        corner_table.estimate(corner_index(fto)),
        edge_table.estimate(dense_edge_index(fto)),
        triangle_table.estimate(tri1_index(fto)),
        triangle_table.estimate(tri2_index(fto)),
        triplet_table.estimate(triplet_index(fto))
    });
}

Solutions<CubieFTO> optimal(const CubieFTO &fto, const unsigned max_depth){
    
    auto root = make_root(fto);
    return IDAstar<true, CubieFTO>(root, estimate, is_solved, standard_directions<CubieFTO>, max_depth);
}

Solutions<FTO> optimal(const FTO &fto, const unsigned max_depth){
    // if (!load_move_tables()) generate_move_tables(); // CHECKME : pourquoi on ne peut pas loader ici ?

    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, estimate, is_solved, standard_directions<FTO>, max_depth);
}