#pragma once
#include <map>
#include "../lib/search.hpp"
#include "solve.hpp"


void generate_corner_table();
void generate_edge_table();
void generate_triangle_table();
void generate_small_pruning_tables();

namespace RLBD {
    constexpr unsigned EDGE_CARD = ipow(3, 4);
    constexpr unsigned CARD = CORNER_CARD * EDGE_CARD;
    constexpr std::array<Move, 8> moves {R, R2, L, L2, B, B2, D, D2};

    Sequence<Move> random_moves(const unsigned &);
    
    struct Finish {
        const fs::path pruning_table_path = table_dir / "rlbd";
        PruningTable<CARD> pruning_table;

        Finish();
        Solutions<FTO> solve(const FTO &, const unsigned m = 23);
    };

    struct Reduction {
        const fs::path triplet_table_path = table_dir / "triplet_reduction";
        const fs::path e1_table_path = table_dir / "edge_comb";
        const fs::path e2_ptable_path = table_dir / "edge_reduction";
        const fs::path triangle_table_path = table_dir / "triangles";

        PruningTable<CORNER_CARD * TRIANGLE_CARD> triplet_ptable;
        PruningTable<EDGE_COMB_CARD> e1_ptable;
        PruningTable<EDGE_COMB_CARD> e2_ptable;
        PruningTable<TRIANGLE_CARD> triangle_ptable;

        Reduction();
        bool is_reduced(const FTO&);
        Solutions<FTO> solve_centers(const FTO&, const unsigned m = 23);
        Solutions<FTO> solve(const FTO &fto, unsigned max_depth = 23);
    };
}; // namespace RLBDnst FTO& fto);