#include "rlbd.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"
#include <set>

namespace RLBD {

std::map<unsigned, unsigned> edge_reduction_map;  // map full space index to rlbd local index
std::array<unsigned, EDGE_CARD> edge_expansion_table; // map local rlbd to full space index

unsigned index(const FTO &fto){
    // The e1 index is always going to be 0 in the RLBD subgroup
    // The edge state is fully determined by the edge comb coordinate of the second orbit
    unsigned ret = corner_index(fto) * EDGE_CARD + edge_reduction_map[e2_index(fto)];
    assert(ret < CARD);
    return ret;
};

void from_index(const unsigned &idx, FTO &fto) {
    unsigned c = idx / EDGE_CARD;
    unsigned e = idx % EDGE_CARD;
    corners_from_index(c, fto);
    e2_from_index(edge_expansion_table[e], fto);
}

void build_edge_reduction_map() {
    auto is_visited = [](const typename Node<FTO>::sptr node) {
        return edge_reduction_map.contains(e2_index(node->state));
    };
    
    auto process = [](const typename Node<FTO>::sptr node) {
        static unsigned reduced = 0;
        unsigned e = e2_index(node->state);
        edge_expansion_table[reduced] = e;
        edge_reduction_map[e] = reduced++;
    };

    BFS_traversal<FTO>(process, is_visited, moves);
    
    assert(edge_reduction_map.size() == EDGE_CARD);
    for (unsigned k = 0; k < EDGE_CARD; ++k) {
        assert(edge_reduction_map[edge_expansion_table[k]] == k);
    }
}

Finish::Finish() {
    // BFS generation of the edge reduction map
    build_edge_reduction_map();

    // Loading optimal pruning table
    if (!pruning_table.load(pruning_table_path)){
        print("Generating RLBD pruning table");
        pruning_table.generate<FTO, true>(index, from_index, moves, 6, 9);
        pruning_table.write(pruning_table_path);
    }
}

const std::vector<Move> &rlbd_allowed_next(const Move &m) {
    // CHECKME : would be nice if this didn't have to be reimplemented for every new puzzle / moveset
    static const std::vector<Move> afterR {L, L2, B, B2, D, D2};
    static const std::vector<Move> afterL {R, R2, B, B2, D, D2};
    static const std::vector<Move> afterB {R, R2, L, L2, D, D2};
    static const std::vector<Move> afterD {R, R2, L, L2, B, B2};
    static const std::vector<Move> dflt {R, R2, L, L2, B, B2, D, D2};


    switch (m) {
        case R ... R2:
            return afterR;
        case L ... L2:
            return afterL;
        case B ... B2:
            return afterB;
        case D ... D2:
            return afterD;
        default:
            return dflt;
    }
}

std::vector<Move> directions(const typename Node<FTO>::sptr node) {
    return allowed_next(static_cast<Move>(node->last_move));
}

Sequence<Move> random_moves(const unsigned &n){
    assert(n > 0);
    srand(time(0));
    Sequence<Move> ret;
    ret.push_back(moves[rand() % NMOVES]);
    for (unsigned k = 0; k < n - 1; ++k) {
        auto next = rlbd_allowed_next(ret.back());
        ret.push_back(next[rand() % next.size()]);
    }
    return ret;
}

Solutions<FTO> Finish::solve(const FTO &fto, const unsigned max_depth){
    // check me : should we make sure the triplets are solved ?
    // CHECKME : Yes but how ?
    assert(e1_index(fto) == 0);
    assert(edge_reduction_map.contains(e2_index(fto)));
    assert(fto.tri1 == 0);

    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, 
        [this](const FTO& fto)->unsigned {return pruning_table.estimate(index(fto));}, 
        is_solved, 
        directions, 
        max_depth);
}

unsigned triplet_index(const FTO& fto) {
    return fto.tri2 * CORNER_CARD + fto.cp;
}

void from_triplet_index(const unsigned &c, FTO &fto) {
    fto.cp = c % CORNER_CARD;
    fto.tri2 = c / CORNER_CARD;
}

Reduction::Reduction() {
    // FIXME : how do we call the global from fto.hpp from within the RLBD namespace ?
    auto all_moves = std::array{U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
    // Optimal edge combination table for orbit 1 (RLBD)
    if (!e1_ptable.load(e1_table_path)){
        print("Generating edge comb pruning table");
        e1_ptable.generate<FTO, true>(e1_index, e1_from_index, all_moves, 3, 7);
        e1_ptable.write(e1_table_path);
    }

    // Optimal triangle table of orbit 1 (RLBD)
    if (!triangle_ptable.load(triangle_table_path)){
        print("Generating triangle pruning table");
        triangle_ptable.generate<FTO, true>(tri1_index, tri1_from_index, all_moves, 3, 7);
        triangle_ptable.write(triangle_table_path);
    }

    // Build the edge reduction table for the second orbit
    if (edge_reduction_map.size() < EDGE_CARD) build_edge_reduction_map();
    for (auto item : RLBD::edge_reduction_map) {
        e2_ptable.set(item.first, 0);
    }
    if (!e2_ptable.load(e2_ptable_path)){
        print("Generating edge reduction pruning table");
        e2_ptable.generate<FTO, true>(e2_index, e2_from_index, all_moves, 1, 5, RLBD::EDGE_CARD);
        e2_ptable.write(e2_ptable_path);
    }

    // Build the table for forming triplets
    auto generators = make_generators<FTO, Move>(RLBD::triplet_index, RLBD::moves);
    assert(generators.size() == CORNER_CARD);
    for (auto g : generators) {
        FTO fto;
        fto.apply(g);
        triplet_ptable.set(RLBD::triplet_index(fto), 0);
    }

    if (!triplet_ptable.load(triplet_table_path)){
        print("Generating triplet forming pruning table");
        triplet_ptable.generate<FTO, true>(RLBD::triplet_index, RLBD::from_triplet_index, all_moves, 3, 7, CORNER_CARD);
        triplet_ptable.write(triplet_table_path);
    }
}

bool centers_solved(const FTO &fto) {
    return fto.tri1 == 0 && fto.e1 == 0 && edge_reduction_map.contains(fto.e2);
}

Solutions<FTO> Reduction::solve_centers(const FTO &fto, const unsigned max_depth) {
    auto root = make_root(fto);
    return IDAstar<true, FTO>(root,
        [this](const FTO& fto){
            return std::max({e1_ptable.estimate(fto.e1),
                e2_ptable.estimate(fto.e2),
                triangle_ptable.estimate(fto.tri1)});
        },
        centers_solved, standard_directions<FTO>, max_depth);
}

// void generate_ptables() {
//     RLBD::generate_edge_map();
//     generate_small_pruning_tables();
//     if (!e2_ptable.load(e2_ptable_path)) generate_e2_ptable();
//     if (!triplet_ptable.load(triplet_table_path)) generate_triplet_ptable();
// }

// bool is_reduced(const FTO& fto) {
//     return centers_solved(fto) && triplet_ptable.estimate(RLBD::triplet_index(fto)) == 0;
// }

Solutions<FTO> Reduction::solve(const FTO &fto, unsigned max_depth) {
    auto root = make_root(fto);
    return IDAstar<true, FTO>(root,
        [this](const FTO& fto){
            return std::max({e1_ptable.estimate(fto.e1),
                e2_ptable.estimate(fto.e2),
                triangle_ptable.estimate(fto.tri1),
                triplet_ptable.estimate(RLBD::triplet_index(fto))
            });
        },
        [this](const FTO& fto)->bool {return (centers_solved(fto) && triplet_ptable.estimate(RLBD::triplet_index(fto)) == 0);},
        standard_directions<FTO>, max_depth);
}
}; // namespace rlbd