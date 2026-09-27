#include <algorithm>
#include "solve.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path triangle_table_path = table_dir / "triangles";
fs::path triplet_table_path = table_dir / "triplets";
fs::path checker_table_path = table_dir / "checker";

PruningTable<CORNER_CARD> corner_table;
PruningTable<EDGE_COMB_CARD> edge_table;
PruningTable<TRIANGLE_CARD> triangle_table;
PruningTable<CORNER_CARD * TRIANGLE_CARD> triplet_table;
PruningTable<CORNER_CARD * EDGE_COMB_CARD> checker_table;

void generate_corner_table(){  
    print("Generating corner pruning table");  
    corner_table.generate<FTO, true>(corner_index, corners_from_index, moves, 3, 4);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){
    print("Generating edge comb pruning table");
    edge_table.generate<FTO, true>(e1_index, e1_from_index, moves, 3, 7);
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
    triplet_table.write(triplet_table_path);
    // triangle_table.show_distribution();
}

void generate_checker_table(){
    // Corners X Edge comb to form checkerboard on the RLBD faces
    print("Generating checker pruning table");
    checker_table.generate<FTO, true>(checker_index, from_checker_index, moves, 7, 10);
    checker_table.write(checker_table_path);
    checker_table.show_distribution();
}

void generate_big_pruning_tables() {
    if (!triplet_table.load(triplet_table_path)) generate_triplet_table();
    if (!checker_table.load(checker_table_path)) generate_checker_table();
};

void generate_small_pruning_tables() {
    if (!corner_table.load(corner_table_path)) generate_corner_table();
    if (!edge_table.load(edge_table_path)) generate_edge_table();
    if (!triangle_table.load(triangle_table_path)) generate_triangle_table();
};

unsigned estimate(const CubieFTO &fto){
    return std::max({corner_table.estimate(fto.corner_index()),
                    // edge_table.estimate(fto.ep.index()),
                    triangle_table.estimate(fto.tri1.index()),
                    triangle_table.estimate(fto.tri2.index())}
        );
};

unsigned estimate(const FTO& fto) {
    return std::max({
        // corner_table.estimate(corner_index(fto)), // those are smaller than
        // triangle_table.estimate(tri1_index(fto)), // the checker and
        // triangle_table.estimate(tri2_index(fto)), // triplet values
        triplet_table.estimate(triplet_index(fto)),
        triplet_table.estimate(triplet2_index(fto)),
        checker_table.estimate(checker_index(fto)),
        checker_table.estimate(checker2_index(fto))
    });
}

unsigned tri1_estimate(const CubieFTO& cfto) {
    return triangle_table.estimate(cfto.tri1.index());
}

unsigned e1_estimate(const FTO& fto) {
    return edge_table.estimate(fto.e1);
}

unsigned tri1_estimate(const FTO& fto) {
    return triangle_table.estimate(fto.tri1);
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