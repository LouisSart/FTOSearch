#include "coordinate_fto.hpp"
#include "rlbd.hpp"
// #include "solve.hpp"
// #include "../lib/permutation.hpp"
// #include "../lib/pruning_table.hpp"
// #include <deque>
// #include <set>
// #include <cstdint>



int main(int argc, const char* argv[]) {
    generate_move_tables();
    RLBD::generate_pruning_table();

    // auto scramble = random_moves(15);
    auto scramble = Sequence<Move>{R,L,B,D2,L,R2,B2,D2,L,R2,B2,D2,B};
    scramble.show();
    
    FTO fto;
    fto.apply(scramble);

    auto f = [&fto](){
        auto solutions = RLBD::optimal(fto);
        solutions.show<Move>();
    };
    time_fn(f);

    return 0;
}