#include "utils.hpp"
#include "fto.hpp"
#include<cassert>

static Sequence sequences[5] {{R, U}, {R, U2}, {R, U, L, F}, {R2, L, R, L2}, {R, B2, R2, D, L2}};

int main(int argc, const char* argv[]) {
    for (const auto seq : sequences) {
        seq.show(false, false); 
        print(":", order(seq));
    }
    return 0;
}