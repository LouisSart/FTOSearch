#include "../lib/search.hpp"
#include "fto.hpp"
#include "coordinate_fto.hpp"

template<typename Cube>
std::vector<Move> standard_directions(const typename Node<Cube>::sptr node) {
    static std::vector<Move> all {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

    if (node->parent == nullptr) {
        return all;
    } else {
        return allowed_next(static_cast<Move>(node->last_move));
    }
}


void generate_corner_table();
void generate_edge_table();
void generate_triangle_table();
void generate_edge_convert_table();

void generate_pruning_tables();
bool load_pruning_tables();
bool load_edge_convert_table();
void write_edge_convert_table();

unsigned estimate(const CubieFTO& fto);
unsigned estimate(const FTO& fto);

Solutions<FTO> optimal(const FTO &, const unsigned m = 23);
Solutions<CubieFTO> optimal(const CubieFTO &, const unsigned m = 23);