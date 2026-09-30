#pragma once
#include "../lib/search.hpp"
#include "fto.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"


template<typename Cube>
const std::vector<Move>& standard_directions(const typename Node<Cube>::sptr node) {
    static const std::vector<Move> all {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

    if (node->parent == nullptr) {
        return all;
    } else {
        return allowed_next(static_cast<Move>(node->last_move));
    }
}

void generate_corner_table();
void generate_edge_table();
void generate_triangle_table();
void generate_small_pruning_tables();

struct Optimal {

    fs::path triplet_table_path;
    fs::path checker_table_path;

    PruningTable<CORNER_CARD * TRIANGLE_CARD> triplet_table;
    PruningTable<CORNER_CARD * EDGE_COMB_CARD> checker_table;

    Optimal();

    void generate_triplet_table();
    void generate_checker_table();
    void generate_pruning_tables();

    unsigned estimate(const FTO& fto);
    unsigned e1_estimate(const FTO& fto);
    unsigned tri1_estimate(const FTO& fto);

    Solutions<FTO> solve(const FTO &, const unsigned m = 23);
};