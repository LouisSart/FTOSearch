#include "rlbd.hpp"
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
    unsigned ret = corner_index(fto) * EDGE_CARD + edge_reduction_map[edge_index(fto)];
    assert(ret < CARD);
    return ret;
};

void from_index(const unsigned &idx, FTO &fto) {
    unsigned c = idx / EDGE_CARD;
    unsigned e = idx % EDGE_CARD;
    corners_from_index(c, fto);
    edges_from_index(edge_expansion_table[e], fto);
}

void generate_edge_map(){
    auto is_visited = [](const typename Node<FTO>::sptr node) {
        return edge_reduction_map.contains(edge_index(node->state));
    };
    
    auto process = [](const typename Node<FTO>::sptr node) {
        static unsigned reduced = 0;
        unsigned e = edge_index(node->state);
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

Solutions<FTO> optimal(const FTO &fto, const unsigned max_depth){
    // check me : should we make sure the triplets are solved ?
    assert(edge_reduction_map.contains(edge_index(fto)));
    assert(fto.tri1 == 0);

    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, estimate, is_solved, directions, max_depth);
}

};

namespace reduction {

constexpr unsigned EDGE_COSET_CARD = EDGE_CARD / RLBD::EDGE_CARD;
PruningTable<EDGE_CARD> edge_ptable;
fs::path edge_table_path = "pruning_tables/edge_reduction";


void generate_edge_ptable() {
    if (!edge_ptable.load(edge_table_path)) {
        print("Generating edge RLBD reduction pruning table...");

        auto generators = make_generators<FTO, Move>(edge_index, RLBD::moves);
        unsigned depth = 0;
        edge_ptable.reset();
        for (auto g : generators) {
            CubieFTO fto;
            fto.edge_apply(g);
            edge_ptable[fto.ep.index()] = depth;
        }

        edge_ptable.generate<CubieFTO, true>(edge_index, edges_from_index, moves, 6, 9, RLBD::EDGE_CARD);
        edge_ptable.write(edge_table_path);
        edge_ptable.show_distribution();
    }
}
};