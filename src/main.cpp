#include "coordinate_fto.hpp"
#include "solve.hpp"
#include "../lib/permutation.hpp"
#include "../lib/pruning_table.hpp"
#include <deque>
#include <set>
#include <cstdint>



int main(int argc, const char* argv[]) {
    generate_move_tables();
    generate_pruning_tables();

    auto scramble = random_moves(13);
    scramble.show();

    FTO fto;
    fto.apply(scramble);

    auto f = [&fto](){
        auto solutions = optimal(fto);
        solutions.show<Move>();
    };
    time_fn(f);

    return 0;
}