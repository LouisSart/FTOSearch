#include "solve.hpp"

//  Small pruning tables are globals
fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path triangle_table_path = table_dir / "triangles";

PruningTable<CORNER_CARD> corner_table;
PruningTable<EDGE_COMB_CARD> edge_table;
PruningTable<TRIANGLE_CARD> triangle_table;

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

void generate_small_pruning_tables() {
    if (!corner_table.load(corner_table_path)) generate_corner_table();
    if (!edge_table.load(edge_table_path)) generate_edge_table();
    if (!triangle_table.load(triangle_table_path)) generate_triangle_table();
};


// Big tables stored in object
Optimal::Optimal() {
    triplet_table_path = table_dir / "triplets";
    checker_table_path = table_dir / "checker";
    generate_pruning_tables();
}

void Optimal::generate_triplet_table(){
    print("Generating triplet pruning table");
    triplet_table.generate<FTO, true>(triplet_index, from_triplet_index, moves, 7, 10);
    triplet_table.write(triplet_table_path);
    // triangle_table.show_distribution();
}

void Optimal::generate_checker_table(){
    // Corners X Edge comb to form checkerboard on the RLBD faces
    print("Generating checker pruning table");
    checker_table.generate<FTO, true>(checker_index, from_checker_index, moves, 7, 10);
    checker_table.write(checker_table_path);
    checker_table.show_distribution();
}

void Optimal::generate_pruning_tables() {
    if (!triplet_table.load(triplet_table_path)) generate_triplet_table();
    if (!checker_table.load(checker_table_path)) generate_checker_table();
};

unsigned Optimal::estimate(const FTO& fto) {
    return std::max({
        triplet_table.estimate(triplet_index(fto)),
        triplet_table.estimate(triplet2_index(fto)),
        checker_table.estimate(checker_index(fto)),
        checker_table.estimate(checker2_index(fto))
    });
}

unsigned Optimal::e1_estimate(const FTO& fto) {
    return edge_table.estimate(fto.e1);
}

unsigned Optimal::tri1_estimate(const FTO& fto) {
    return triangle_table.estimate(fto.tri1);
}

Solutions<FTO> Optimal::solve(const FTO &fto, const unsigned max_depth){
    // #include <functional>
    #include <typeinfo>
    
    auto root = make_root(fto);

    return IDAstar<true, FTO>(root,
                // std::bind(&Optimal::estimate, this, std::placeholders::_1), // uses header <functional>
                [this](const FTO& fto)-> unsigned {return this->estimate(fto);},
                is_solved,
                standard_directions<FTO>,
                max_depth);
}