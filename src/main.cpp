// #include "coordinate_fto.hpp"
// #include "rlbd.hpp"
#include "solve.hpp"
// #include "../lib/permutation.hpp"
// #include "../lib/pruning_table.hpp"
// #include <deque>
// #include <set>
// #include <iostream>
// #include <cstdio>
// #include <cassert>
// #include <functional>

int main(int argc, const char* argv[]) {
    // RLBD::generate_pruning_table();
    // reduction::generate_ptables();
    generate_move_tables();

    auto scramble = random_moves(13);
    scramble.show();

    FTO fto;
    fto.apply(scramble);

    auto solver = Optimal();
    auto f = [&fto, &solver](){
        auto solutions = solver.solve(fto);
        solutions.show<Move>();
    };
    time_fn(f);

    return 0;
}