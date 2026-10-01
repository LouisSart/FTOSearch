#include "rlbd.hpp"
#include "solve.hpp"
#include "../lib/option.hpp"
// #include "../lib/permutation.hpp"
// #include "../lib/pruning_table.hpp"
// #include <deque>
// #include <set>
// #include <iostream>
// #include <cstdio>
#include <cstring>
// #include <cassert>
// #include <functional>

int main(int argc, const char* argv[]) {
    generate_move_tables();

    if (argc > 1) {
        if (strcmp(argv[1], "optimal") == 0) {
            auto scramble = random_moves(13);
            scramble.show();

            FTO fto;
            fto.apply(scramble);

            auto solver = Optimal();
            time_fn([&fto, &solver](){
                auto solutions = solver.solve(fto);
                solutions.show<Move>();
            });
        }
        else if (strcmp(argv[1], "rlbd") == 0) {
            RLBD::generate_pruning_table();
        }
        else if (strcmp(argv[1], "reduction") == 0) {
            reduction::generate_ptables();
        } else {
            print("Wrong command line arguments found");
            print("Options : 'optimal', 'rlbd' and 'reduction'");
        }
    } else {
        print("No command line arguments found");
        print("Options : 'optimal', 'rlbd' and 'reduction'");
    }

    return 0;
}