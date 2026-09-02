#include "../lib/search.hpp"
#include "../lib/pruning_table.hpp"
#include "coordinate_fto.hpp"
#include "solve.hpp"


int main(int argc, const char* argv[]) {
    load_move_tables();
    load_pruning_tables();
    // time_fn(generate_edge_table, 1);

    FTO fto;
    fto.random_moves(12);
    auto root = make_root(fto);
    auto solutions = IDAstar<true, FTO>(root, estimate, is_solved, standard_directions<FTO>, 12);
    solutions.show<Move>();

    return 0;
}