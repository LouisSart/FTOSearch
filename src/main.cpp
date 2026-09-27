// #include "coordinate_fto.hpp"
#include "rlbd.hpp"
// #include "solve.hpp"
// #include "../lib/permutation.hpp"
// #include "../lib/pruning_table.hpp"
// #include <deque>
// #include <set>
// #include <cstdint>
#include <cstdio>
#include <cassert>



int main(int argc, const char* argv[]) {
    generate_move_tables();
    // RLBD::generate_pruning_table();
    // generate_big_pruning_tables();
    reduction::generate_ptables();

    auto scramble = random_moves(16);
    scramble.show();

    FTO fto;
    fto.apply(scramble);

    auto f = [&fto](){
        auto solutions = reduction::optimal(fto);
        solutions.show<Move>();
    };
    time_fn(f);

    return 0;
}