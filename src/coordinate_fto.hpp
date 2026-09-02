#include "fto.hpp"
#include "move.hpp"

struct FTO {
    // Coordinate level representation of an FTO
    unsigned cp{0};   // Corner permutation + orientation
    unsigned e1{0};   // 5 edges partial permutation
    unsigned e2{0};   // 5 other edges partial permutation
    unsigned tri1{0}; // Triangles of first tetrad
    unsigned tri2{0}; // Triangles of second tetrad

    unsigned corner_index() const;
    void apply(const Move &m);
};

bool is_solved(const FTO &fto);
unsigned corner_index(const FTO& fto);
void corners_from_index(const unsigned &c, FTO&);
unsigned edge_index(const FTO& fto);
void edges_from_index(const unsigned &c, FTO&);
unsigned tri1_index(const FTO& fto);
void tri1_from_index(const unsigned &c, FTO&);

bool load_move_tables();
void generate_move_tables();