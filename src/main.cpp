#include "table.hpp"
#include "fto.hpp"
#include "search.hpp"

std::vector<Move> standard_directions(const Node<CubieFTO>::sptr node) {
    static std::vector<Move> all {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

    if (node->parent == nullptr) {
        return all;
    } else {
        return allowed_next(static_cast<Move>(node->last_move));
    }
}

int main(int argc, const char* argv[]) {

    load_corner_table();
    load_triangle_table();
    load_edge_table();

    CubieFTO fto;
    // fto.apply({U, R, L, R, bR, D2, bL2, F, D2, R2, U, D2, bR2, L2});
    fto.random_moves(14);
    auto root = make_root(fto);
    auto solutions = IDAstar(root, apply, estimate, is_solved, standard_directions, 14);
    solutions.show<Move>();
    return 0;
}