#include "solve.hpp"
#include "table.hpp"

std::vector<Move> standard_directions(const Node<CubieFTO>::sptr node) {
    static std::vector<Move> all {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

    if (node->parent == nullptr) {
        return all;
    } else {
        return allowed_next(static_cast<Move>(node->last_move));
    }
}

Solutions<NodePtr> optimal(const CubieFTO &fto){
    load_corner_table();
    load_triangle_table();
    load_edge_table();
    
    
    auto root = make_root(fto);
    return IDAstar(root, apply, estimate, is_solved, standard_directions, 14);
}