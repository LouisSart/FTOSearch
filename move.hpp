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