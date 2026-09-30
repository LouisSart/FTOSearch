#include "solve.hpp"

const fs::path table_dir = "pruning_tables";

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