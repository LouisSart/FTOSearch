#pragma once
#include "permutation.hpp"
#include "utils.hpp"
#include "move.hpp"

constexpr unsigned NC = 6;
constexpr unsigned NE = 12;
constexpr unsigned NT = 12;

struct CubieFTO {
    Permutation<NC> cp;   // Corner permutation
    Orientation<NC> co;   // Corner orientation
    Permutation<NE> ep;   // Edge permutation
    Triangles<NT, 3> tri1; // Triangles of first tetrad
    Triangles<NT, 3> tri2; // Triangles of second tetrad

    void corner_apply(const Move& m);
    void corner_apply(const Sequence &seq);
    void edge_apply(const Move& m);
    void edge_apply(const Sequence &seq);
    void triangle_apply(const Move& m);
    void triangle_apply(const Sequence &seq);
    void apply(const Move &m);
    void apply(const Sequence &seq);
    bool is_solved() const;
    unsigned corner_index() const;
    void set_corners_from_index(const unsigned &c);
};

unsigned order(const Sequence &seq);