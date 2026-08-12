#include "utils.hpp"
#include "move.hpp"
#include<cassert>

int main(int argc, const char* argv[]) {
    print(order({R, U}));
    print(order({R, U2}));
    print(order({R, U, L, F}));
    print(order({R2, L, R, L2}));
    print(order({R, B2, R2, D, L2}));
    return 0;
}