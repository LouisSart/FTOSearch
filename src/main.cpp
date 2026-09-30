// #include "coordinate_fto.hpp"
// #include "rlbd.hpp"
#include "solve.hpp"
// #include "../lib/permutation.hpp"
// #include "../lib/pruning_table.hpp"
// #include <deque>
// #include <set>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <functional>

struct Foo {
    int a(int arg){return arg*arg;}
};

int main(int argc, const char* argv[]) {
    generate_move_tables();
    // // RLBD::generate_pruning_table();
    // // generate_big_pruning_tables();
    // // reduction::generate_ptables();

    auto solver = Optimal();
    solver.generate_big_pruning_tables();
    auto scramble = random_moves(16);
    scramble.show();

    FTO fto;
    fto.apply(scramble);

    auto f = [&fto, &solver](){
        auto solutions = solver.solve(fto);
        solutions.show<Move>();
    };
    time_fn(f);

    return 0;
}