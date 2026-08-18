#include "permutation.hpp"
#include "utils.hpp"
#include <typeinfo>

constexpr unsigned N = 12, K = 0, M = 4, NP = 3;

template<std::size_t n>
void foo(std::array<unsigned, n>){}

int main(int argc, const char* argv[]) {
    const unsigned c = 1000;
    std::array<unsigned, N - K * M> loc_layout;
    print(typeid(c % binomial(N - K * NP, NP)).name());
    print(typeid(NP).name());
    print(typeid(N - K * M).name());
    // layout_from_index(c % binomial(N - K * NP, NP), loc_layout, NP);
    foo(loc_layout);

    return 0;
}