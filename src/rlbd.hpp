#pragma once
#include "coordinate_fto.hpp"
#include "../lib/search.hpp"
#include "solve.hpp"

void generate_corner_table();
void generate_edge_table();
void generate_triangle_table();
void generate_small_pruning_tables();

namespace RLBD {
    void generate_pruning_table();
    Solutions<FTO> optimal(const FTO &, const unsigned m = 23);
};

namespace reduction {
unsigned e1_estimate(const FTO& fto);
unsigned tri1_estimate(const FTO& fto);
Solutions<FTO> solve_centers(const FTO&, const unsigned m = 23);
bool is_solved(const FTO& fto);
void generate_triplet_ptable();
void generate_e2_ptable();
void generate_ptables();
Solutions<FTO> optimal(const FTO &fto, unsigned max_depth = 23);
};