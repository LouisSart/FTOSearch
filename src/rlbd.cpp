#include "rlbd.hpp"
#include "solve.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"
#include <map>


namespace fs = std::filesystem;
namespace RLBD {

    constexpr unsigned EDGE_CARD = ipow(3, 4);
    constexpr unsigned CARD = CORNER_CARD * EDGE_CARD;
    std::map<unsigned, unsigned> edge_reduction_map;  // map full space index to rlbd local index
    std::array<unsigned, EDGE_CARD> edge_expansion_table; // map local rlbd to full space index
    std::array<Move, 8> moves {R, R2, L, L2, B, B2, D, D2};
    PruningTable<CARD> pruning_table;
    fs::path pruning_table_path = "pruning_tables/rlbd";
    
    unsigned index(const FTO &fto){
        unsigned ret = corner_index(fto) * EDGE_CARD + edge_reduction_map[dense_edge_index(fto)];
        assert(ret < CARD);
        return ret;
    };
    
    void from_index(const unsigned &idx, FTO &fto) {
        unsigned c = idx / EDGE_CARD;
        unsigned e = idx % EDGE_CARD;
        corners_from_index(c, fto);
        edges_from_dense_index(edge_expansion_table[e], fto);
    }
    
    void generate_edge_map(){
        auto is_visited = [](const typename Node<FTO>::sptr node) {
            return edge_reduction_map.contains(dense_edge_index(node->state));
        };
    
    auto process = [](const typename Node<FTO>::sptr node) {
            static unsigned reduced = 0;
            edge_expansion_table[reduced] = dense_edge_index(node->state);
            edge_reduction_map[dense_edge_index(node->state)] = reduced++;
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

};