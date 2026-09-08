#include "coordinate_fto.hpp"
#include "solve.hpp"
#include "../lib/permutation.hpp"
#include "../lib/pruning_table.hpp"
#include <deque>
#include <set>
#include <cstdint>



int main(int argc, const char* argv[]) {
    load_move_tables();
    load_pruning_tables();

    auto scramble = random_moves(18);
    scramble.show();
    CubieFTO cfto;
    cfto.apply(scramble);
    FTO fto(cfto);

    auto solutions = optimal(fto);
    solutions.show<Move>();

    return 0;
}