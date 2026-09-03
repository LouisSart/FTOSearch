#pragma once
#include "fto.hpp"
#include "move.hpp"

static constexpr unsigned SIX_EDGE_PERM_CARD = Permutation<6>::CARD;
static constexpr unsigned SIX_EDGE_CARD = Layout<NE, 6>::CARD * SIX_EDGE_PERM_CARD;

struct FTO {
    // Coordinate level representation of an FTO
    unsigned cp{0};   // Corner permutation + orientation
    unsigned e1{0};   // 6 edges partial permutation
    unsigned e2{0};   // 6 other edges partial permutation
    unsigned tri1{0}; // Triangles of first tetrad
    unsigned tri2{0}; // Triangles of second tetrad

    unsigned corner_index() const;
    void apply(const Move &m);
    void show() const;
    void random_moves(const unsigned n);
};

bool is_solved(const FTO &);
unsigned corner_index(const FTO&);
void corners_from_index(const unsigned &, FTO&);
unsigned e1_index(const CubieFTO&);
unsigned e2_index(const CubieFTO&);
unsigned edge_index(const FTO&);
void edges_from_index(const unsigned &, FTO&);
unsigned tri1_index(const FTO&);
void tri1_from_index(const unsigned &, FTO&);
unsigned tri2_index(const FTO&);
void tri2_from_index(const unsigned &, FTO&);
bool load_move_tables();
void generate_move_tables();