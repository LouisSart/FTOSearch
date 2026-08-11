#pragma once
#include "utils.hpp"

constexpr unsigned NMOVES = 8;
constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12;

enum Moves : unsigned {U, R, F, L, B, bR, D, bL};

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
    {2,1,3,0,4,5}, // R
    {1,2,0,3,4,5}, // F
    {4,0,2,3,1,5}, // L
    {0,1,2,5,3,4}, // B
    {0,1,5,2,4,3}, // bR
    {0,5,1,3,4,2}, // D
    {0,4,2,3,5,1}  // bL
};

// Edge permutations
static Permutation<NE> EP[NMOVES] {
    {1,6,2,3,4,5,0,7,8,9,10,11}, // U
    {0,2,5,3,4,1,6,7,8,9,10,11}, // R
    {0,1,3,4,2,5,6,7,8,9,10,11}, // F
    {7,1,2,0,4,5,6,3,8,9,10,11}, // L
    {0,1,2,3,4,5,10,7,8,9,11,6}, // B
    {0,1,2,3,4,9,6,7,8,10,5,11}, // bR
    {0,1,2,3,8,5,6,7,9,4,10,11}, // D
    {0,1,2,3,4,5,6,11,7,9,10,8},  // bL
};