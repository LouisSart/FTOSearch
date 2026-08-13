#include "utils.hpp"
#include "fto.hpp"
#include <cassert>

static Sequence sequences[5] {{R, U}, {R, U2}, {R, U, L, F}, {R2, L, R, L2}, {R, B2, R2, D, L2}};
static Sequence moves {R, U, D, F2, bR2, L, D2, F, B, bL2, U, bL2};

int main(int argc, const char* argv[]) {
    for (const auto seq : sequences) {
        seq.show(false, false);
        auto n = order(seq);
        print(":", n);
    }
    return 0;
}