#include "rlbd.hpp"
#include "solve.hpp" // triangle estimate
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"
#include <map>
#include <set>


namespace fs = std::filesystem;
namespace RLBD {

constexpr unsigned EDGE_CARD = ipow(3, 4);
constexpr unsigned CARD = CORNER_CARD * EDGE_CARD;
std::map<unsigned, unsigned> edge_reduction_map;  // map full space index to rlbd local index
std::array<unsigned, EDGE_CARD> edge_expansion_table; // map local rlbd to full space index
constexpr std::array<Move, 8> moves {R, R2, L, L2, B, B2, D, D2};
PruningTable<CARD> pruning_table;
fs::path pruning_table_path = "pruning_tables/rlbd";

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

void generate_edge_map(){
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

void generate_pruning_table() {
    generate_edge_map();
    if (!pruning_table.load(pruning_table_path)){
        print("Generating RLBD pruning table");
        pruning_table.generate<FTO, true>(index, from_index, moves, 6, 9);
        pruning_table.write(pruning_table_path);
    }
}

unsigned estimate(const FTO &fto) {
    return pruning_table.estimate(index(fto));
}

std::array<Move, 8> directions(const typename Node<FTO>::sptr node) {
    return moves;
}

Sequence<Move> random_moves(const unsigned &n){
    assert(n > 0);
    srand(time(0));
    Sequence<Move> ret;
    ret.push_back(moves[rand() % NMOVES]);
    for (unsigned k = 0; k < n - 1; ++k) {
        auto next = allowed_next(ret.back());
        ret.push_back(next[rand() % next.size()]);
    }
    return ret;
}

Solutions<FTO> optimal(const FTO &fto, const unsigned max_depth){
    // check me : should we make sure the triplets are solved ?
    // CHECKME : Yes but how ?
    assert(e1_index(fto) == 0);
    assert(edge_reduction_map.contains(e2_index(fto)));
    assert(fto.tri1 == 0);

    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, estimate, is_solved, directions, max_depth);
}

}; // namespace rlbd

namespace reduction {

fs::path e2_ptable_path = "pruning_tables/edge2_reduction";
PruningTable<EDGE_COMB_CARD> e2_ptable;

// bool is_visited (const typename Node<FTO>::sptr node){
//     return e2_ptable.is_assigned(node->state.e2);
// };
// void process_depth_zero(const typename Node<FTO>::sptr node){
//     e2_ptable.set(node->state.e2, 0);
// };

void generate_e2_ptable() {
    for (auto item : RLBD::edge_reduction_map) {
        e2_ptable.set(item.first, 0);
    }
    if (!e2_ptable.load(e2_ptable_path)){
        print("Generating edge reduction pruning table");
        e2_ptable.generate<FTO, true>(e2_index, e2_from_index, moves, 1, 5, RLBD::EDGE_CARD);
        e2_ptable.write(e2_ptable_path);
    }
}

bool centers_solved(const FTO &fto) {
    return fto.tri1 == 0 && fto.e1 == 0 && RLBD::edge_reduction_map.contains(fto.e2);
}

unsigned center_estimate(const FTO &fto) {
    return std::max({e1_estimate(fto),
                e2_ptable.estimate(fto.e2),
                tri1_estimate(fto)});
}

Solutions<FTO> solve_centers(const FTO &fto, const unsigned max_depth) {
    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, center_estimate, centers_solved, standard_directions<FTO>, max_depth);
}


// std::set<unsigned> gen_set;
// std::vector<Sequence<Move>> generators;

// void edge_coset() {
//     std::array<unsigned, EDGE_COMB_CARD> edge_coset_table;
//     generate_right_coset_table<FTO, Move, EDGE_COMB_CARD>(e2_index, generators, moves, edge_coset_table);
// }
}; // namespace reduction