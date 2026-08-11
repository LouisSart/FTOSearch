#pragma once
#include "utils.hpp"

constexpr unsigned NMOVES = 16;
constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12;

enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
using Sequence = std::vector<Move>;

template<unsigned N>
struct Permutation : std::array<unsigned, N> {
    Permutation() { // Constructeur sans arguments
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }
    template<typename... Args>
    Permutation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k){
            if (this->operator[](k) != k) return false;
        }
        return true;
    }
    void reset() {
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }
    void compose(const Permutation<N> & other) {
        Permutation<N> ret = *this;
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = ret[other[k]];
        }
    }
};

template<typename T, unsigned N>
std::array<T, N> permute(const std::array<T, N> &items, const Permutation<NC> &perm) {
    std::array<T, N> ret = items;
    for (unsigned k = 0; k < N; ++k){
        ret(k) = items[perm[k]];
    }
    return ret;
}

// Corner permutations
static Permutation<NC> CP[NMOVES] {
    {3,1,2,4,0,5}, // U
    {4,1,2,0,3,5}, // U'
    {2,1,3,0,4,5}, // R
    {3,1,0,2,4,5}, // R'
    {1,2,0,3,4,5}, // F
    {2,0,1,3,4,5}, // F'
    {4,0,2,3,1,5}, // L
    {1,4,2,3,0,5}, // L'
    {0,1,2,5,3,4}, // B
    {0,1,2,4,5,3}, // B'
    {0,1,5,2,4,3}, // bR
    {0,1,3,5,4,2}, // bR'
    {0,5,1,3,4,2}, // D
    {0,2,5,3,4,1}, // D'
    {0,4,2,3,5,1}, // bL
    {0,5,2,3,1,4}, // bL'
};

void corner_apply(Permutation<NC> &p, const Sequence &seq) {
    for (auto m : seq){
        p.compose(CP[m]);
    }
}

// Edge permutations
static Permutation<NE> EP[NMOVES] {
    {1,6,2,3,4,5,0,7,8,9,10,11}, // U
    {6,0,2,3,4,5,1,7,8,9,10,11}, // U'
    {0,2,5,3,4,1,6,7,8,9,10,11}, // R
    {0,5,1,3,4,2,6,7,8,9,10,11}, // R'
    {0,1,3,4,2,5,6,7,8,9,10,11}, // F
    {0,1,4,2,3,5,6,7,8,9,10,11}, // F'
    {7,1,2,0,4,5,6,3,8,9,10,11}, // L
    {3,1,2,7,4,5,6,0,8,9,10,11}, // L'
    {0,1,2,3,4,5,10,7,8,9,11,6}, // B
    {0,1,2,3,4,5,11,7,8,9,6,10}, // B'
    {0,1,2,3,4,9,6,7,8,10,5,11}, // bR
    {0,1,2,3,4,10,6,7,8,5,9,11}, // bR
    {0,1,2,3,8,5,6,7,9,4,10,11}, // D
    {0,1,2,3,9,5,6,7,4,8,10,11}, // D'
    {0,1,2,3,4,5,6,11,7,9,10,8},  // bL
    {0,1,2,3,4,5,6,8,11,9,10,7}, // bL'
};

void edge_apply(Permutation<NE> &p, const Sequence &seq) {
    for (auto m : seq){
        p.compose(EP[m]);
    }
}
