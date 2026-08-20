#pragma once
#include "permutation.hpp"
#include "utils.hpp"
#include "move.hpp"

constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12; // 12 triangles per tetrad
constexpr unsigned NF = 4; // 4 faces per tetrad

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
};

unsigned order(const Sequence<Move> &seq);
void apply(const Move &m, CubieFTO &fto);;
unsigned corner_index(const CubieFTO& fto);
CubieFTO corners_from_index(const unsigned &c);
void corner_apply(const Move &m, CubieFTO& fto);
unsigned edge_index(const CubieFTO& fto);
CubieFTO edges_from_index(const unsigned &c);
void edge_apply(const Move &m, CubieFTO& fto);
unsigned tri1_index(const CubieFTO& fto);
CubieFTO tri1_from_index(const unsigned &c);
void triangle_apply(const Move &m, CubieFTO& fto);
