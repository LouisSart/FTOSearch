#include "fto.hpp"
#include <cassert>

// Corner permutations
static const Permutation<NC> CP[NMOVES] {
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

static const Orientation<NC> CO[NMOVES] {
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
static const Permutation<NE> EP[NMOVES] {
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
static const Permutation<NT> TP[NMOVES] {
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

static const Move zSHIFT[NMOVES] {L, L2, U, U2, R, R2, F, F2, bL, bL2, B, B2, bR, bR2, D, D2};

// struct CubieFTO {
//     Permutation<NC> cp;   // Corner permutation
//     Orientation<NC> co;   // Corner orientation
//     Permutation<NE> ep;   // Edge permutation
//     Triangles<NT, 3> tri1; // Triangles of first tetrad
//     Triangles<NT, 3> tri2; // Triangles of second tetrad

    
// };

void CubieFTO::corner_apply(const Move& m) {
    cp.compose(CP[m]);
    permute<NC>(co, CP[m]);
    for (unsigned k = 0; k < NC; ++k) {
        co[k] = (co[k] + CO[m][k]) % 2;
    }
};

void CubieFTO::corner_apply(const Sequence<Move> &seq) {
    for (auto m : seq){
        corner_apply(m);
    }
}
void CubieFTO::edge_apply(const Move& m){
    ep.compose(EP[m]);
}

void CubieFTO::edge_apply(const Sequence<Move> &seq) {
    for (auto m : seq){
        edge_apply(m);
    }
}

void CubieFTO::triangle_apply(const Move& m){
    permute<NT>(tri1, TP[m]);
    permute<NT>(tri2, TP[zSHIFT[m]]); // second tetrad is the same as the first through a z shift
}
    
void CubieFTO::triangle_apply(const Sequence<Move> &seq) {
    for (auto m : seq){
        triangle_apply(m);
    }
}
void CubieFTO::apply(const Move &m){
    corner_apply(m);
    edge_apply(m);
    triangle_apply(m);
}
void CubieFTO::apply(const Sequence<Move> &seq){
    corner_apply(seq);
    edge_apply(seq);
    triangle_apply(seq);
}

bool CubieFTO::is_solved() const {
    return cp.is_solved() && co.is_solved() && ep.is_solved() && tri1.is_solved() && tri2.is_solved();
}
unsigned CubieFTO::corner_index() const {
    // Return the index for cp + co
    return co.index() * cp.cardinality() + cp.index();
}
void CubieFTO::set_corners_from_index(const unsigned &c) {
    unsigned coc = c / cp.cardinality();
    unsigned cpc = c % cp.cardinality();
    cp.set_from_index(cpc);
    co.set_from_index(coc);
}

unsigned order(const Sequence<Move> &seq) {
    CubieFTO fto;
    fto.apply(seq);
    unsigned count = 1;
    while (!fto.is_solved()){
        fto.apply(seq);
        count += 1;
    }
    return count;
}

void apply(const Move &m, CubieFTO &fto) {
    fto.apply(m);
}

bool is_solved(const CubieFTO &fto){
    return fto.is_solved();
}

// CORNERS WITH ORIENTATION
unsigned corner_index(const CubieFTO& fto){
    return fto.corner_index();
}

CubieFTO corners_from_index(const unsigned &c) {
    CubieFTO fto;
    fto.set_corners_from_index(c);
    return fto;
}

void corner_apply(const Move &m, CubieFTO& fto){
    fto.corner_apply(m);
}


// EDGES
unsigned edge_index(const CubieFTO& fto){
    return fto.ep.index();
}

CubieFTO edges_from_index(const unsigned &c) {
    CubieFTO fto;
    fto.ep.set_from_index(c);
    return fto;
}

void edge_apply(const Move &m, CubieFTO& fto){
    fto.edge_apply(m);
}

// TRIANGLES
unsigned tri1_index(const CubieFTO& fto){
    return fto.tri1.index();
}

CubieFTO tri1_from_index(const unsigned &c) {
    CubieFTO fto;
    fto.tri1.set_from_index(c);
    return fto;
}

void triangle_apply(const Move &m, CubieFTO& fto){
    fto.triangle_apply(m);
}
