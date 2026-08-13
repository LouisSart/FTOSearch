#pragma once
#include "utils.hpp"

constexpr unsigned NMOVES = 16;
constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12;

enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

struct Sequence : std::vector<Move> {
    template<typename... Args>
    Sequence(Args... args) : std::vector<Move>{{ args... }} {} // Constructeur par brace-enclosed
    void show(const bool show_length=true, const bool line_break=true) const {
        static const std::string notation[NMOVES]
            {"U", "U'", "R", "R'", "F", "F'", "L", "L'", "B", "B'", "bR", "bR'", "D", "D'", "bL", "bL'"};
        
        for (const auto m : *this) {
            std::cout << notation[m] << " ";
        }
        
        if (show_length) std::cout << "(" << size() << ")";
        if (line_break) std::cout << std::endl;
    }
};

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
    unsigned index(const bool even = false) const {
        // Compute the lexicographic index of the permutation

        // If the permutation has even parity,
        // do not encode the information of the last two digits
        unsigned n;
        if (even) n = N - 1;
        else n = N;

        unsigned t = 0;
        for (unsigned i = 0; i < n - 1; ++i){
            t = t * (N - i);
            for (unsigned j = i; j < n; ++j) {
                if ((*this)[i] > (*this)[j]) t = t + 1;
            }
        }
        return t;
    }
    void set_from_index(const unsigned &c, const bool even = false){

    }
};

template<unsigned N>
struct Orientation : std::array<unsigned, N> {
    Orientation() { // Constructeur sans arguments
        std::array<unsigned, N>::fill(0);
    }
    template<typename... Args>
    Orientation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    bool is_solved() const {
        for (unsigned k : *this){
            if (k != 0) return false;
        }
        return true;
    }
};

struct Triangles : std::array<unsigned, NT> {
    // Solved_state is {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3}
    Triangles() {
        for (unsigned k = 0; k < NT; ++k) {
            this->operator[](k) = k / 3;
        }
    }

    bool is_solved() const {
        for (unsigned k = 0; k < NT; ++k) {
            if (this->operator[](k) != k / 3) return false;
        }
        return true;
    }
};

template<unsigned N>
void permute(std::array<unsigned, N> &items, const Permutation<N> &perm) {
    std::array<unsigned, N> buf = items;
    for (unsigned k = 0; k < N; ++k){
        items[k] = buf[perm[k]];
    }
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

static Orientation<NC> CO[NMOVES] {
    {1,0,0,1,0,0}, // U
    {0,0,0,1,1,0}, // U'
    {0,0,0,0,0,0}, // R
    {0,0,0,0,0,0}, // R'
    {0,1,1,0,0,0}, // F
    {1,0,1,0,0,0}, // F'
    {1,1,0,0,0,0}, // L
    {1,0,0,0,1,0}, // L'
    {0,0,0,0,0,0}, // B
    {0,0,0,0,0,0}, // B'
    {0,0,1,1,0,0}, // bR
    {0,0,1,0,0,1}, // bR'
    {0,1,0,0,0,1}, // D
    {0,0,1,0,0,1}, // D'
    {0,1,0,0,1,0}, // bL
    {0,0,0,0,1,1}, // bL'
};

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

// Triangles permutation
static Permutation<NT> TP[NMOVES] {
    {7,8,2,1,4,0,6,5,3,9,10,11}, // U
    {5,3,2,8,4,7,6,0,1,9,10,11}, // U'
    {2,0,1,3,4,5,6,7,8,9,10,11}, // R
    {1,2,0,3,4,5,6,7,8,9,10,11}, // R'
    {4,1,3,9,10,5,6,7,8,2,0,11}, // F
    {10,1,9,2,0,5,6,7,8,3,4,11}, // F'
    {0,1,2,5,3,4,6,7,8,9,10,11}, // L
    {0,1,2,4,5,3,6,7,8,9,10,11}, // L'
    {0,1,2,3,4,5,8,6,7,9,10,11}, // B
    {0,1,2,3,4,5,7,8,6,9,10,11}, // B'
    {0,10,11,3,4,5,1,2,8,9,6,7}, // bR
    {0,6,7,3,4,5,10,11,8,9,1,2}, // bR'
    {0,1,2,3,4,5,6,7,8,11,9,10}, // D
    {0,1,2,3,4,5,6,7,8,10,11,9}, // D'
    {0,1,2,3,8,6,9,7,11,5,10,4}, // bL
    {0,1,2,3,11,9,5,7,4,6,10,8}  // bL'
};

static Move zSHIFT[NMOVES] {L, L2, U, U2, R, R2, F, F2, bL, bL2, B, B2, bR, bR2, D, D2};

struct CubieFTO {
    Permutation<NC> cp;   // Corner permutation
    Orientation<NC> co;   // Corner orientation
    Permutation<NE> ep;   // Edge permutation
    Triangles tri1; // Triangles of first tetrad
    Triangles tri2; // Triangles of second tetrad

    void corner_apply(const Move& m) {
        cp.compose(CP[m]);
        permute<NC>(co, CP[m]);
        for (unsigned k = 0; k < NC; ++k) {
            co[k] = (co[k] + CO[m][k]) % 2;
        }
    };

    void corner_apply(const Sequence &seq) {
        for (auto m : seq){
            corner_apply(m);
        }
    }

    void edge_apply(const Move& m){
        ep.compose(EP[m]);
    }

    void edge_apply(const Sequence &seq) {
        for (auto m : seq){
            edge_apply(m);
        }
    }

    void triangle_apply(const Move& m){
        permute<NT>(tri1, TP[m]);
        permute<NT>(tri2, TP[zSHIFT[m]]); // second tetrad is the same as the first through a z shift
    }

    void triangle_apply(const Sequence &seq) {
        for (auto m : seq){
            triangle_apply(m);
        }
    }

    void apply(const Move &m){
        corner_apply(m);
        edge_apply(m);
        triangle_apply(m);
    };

    void apply(const Sequence &seq){
        corner_apply(seq);
        edge_apply(seq);
        triangle_apply(seq);
    };

    bool is_solved() const {
        return cp.is_solved() && co.is_solved() && ep.is_solved() && tri1.is_solved() && tri2.is_solved();
    }
};

unsigned order(const Sequence &seq) {
    CubieFTO fto;
    fto.apply(seq);
    unsigned count = 1;
    while (!fto.is_solved()){
        fto.apply(seq);
        count += 1;
    }
    return count;
}