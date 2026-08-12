#include "utils.hpp"
#include "move.hpp"
#include<cassert>

static Sequence sequences[5] {{R, U}, {R, U2}, {R, U, L, F}, {R2, L, R, L2}, {R, B2, R2, D, L2}};

int main(int argc, const char* argv[]) {
    for (const auto seq : sequences) {
        print(seq, false, false); 
        print(":", order(seq));
    }
    return 0;
}