#pragma once
#include "../lib/permutation.hpp"
#include "../lib/utils.hpp"
#include "../lib/move.hpp"

enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
std::ostream& operator<< (std::ostream& out, const Move& m);
constexpr unsigned NMOVES = 16;
constexpr Move moves[NMOVES] = {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

// Reduce branching by preventing turning twice the same layer / 
// chaining opposite moves
const std::vector<Move> &allowed_next(const Move m);


constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12; // 12 triangles per tetrad
constexpr unsigned NF = 4; // 4 faces per tetrad

constexpr unsigned CP_CARD = Permutation<6, true>::CARD;
constexpr unsigned CO_CARD = Orientation<6>::CARD;
constexpr unsigned CORNER_CARD = CP_CARD * CO_CARD;
constexpr unsigned EDGE_CARD = Permutation<12, true>::CARD;
constexpr unsigned TRIANGLE_CARD = Center<12, 4>::CARD;

static const Move zSHIFT[NMOVES] {L, L2, U, U2, R, R2, F, F2, bL, bL2, B, B2, bR, bR2, D, D2};

struct CubieFTO {
    Permutation<NC, true> cp;   // Corner permutation
    Orientation<NC> co;   // Corner orientation
    Permutation<NE, true> ep;   // Edge permutation
    Center<NT, NF> tri1; // Triangles of first tetrad
    Center<NT, NF> tri2; // Triangles of second tetrad

    void corner_apply(const Move& m);
    void corner_apply(const Sequence<Move> &seq);
    void edge_apply(const Move& m);
    void edge_apply(const Sequence<Move> &seq);
    void triangle_apply(const Move& m);
    void triangle_apply(const Sequence<Move> &seq);
    void apply(const Move &m);
    void apply(const Sequence<Move> &seq);
    bool is_solved() const;
    unsigned corner_index() const;
    void set_corners_from_index(const unsigned &c);
    void random_moves(const unsigned &n);
};

unsigned order(const Sequence<Move> &seq);
bool is_solved(const CubieFTO &fto);
unsigned corner_index(const CubieFTO& fto);
void corners_from_index(const unsigned &c, CubieFTO& fto);
unsigned edge_index(const CubieFTO& fto);
void edges_from_index(const unsigned &c, CubieFTO& fto);
unsigned tri1_index(const CubieFTO& fto);
void tri1_from_index(const unsigned &c, CubieFTO& fto);
unsigned tri2_index(const CubieFTO&);
void tri2_from_index(const unsigned &c, CubieFTO &);

