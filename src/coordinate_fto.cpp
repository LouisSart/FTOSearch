#include <cstdlib>
#include <ctime>
#include "move_table.hpp"
#include "../lib/permutation.hpp"
#include "../lib/search.hpp" // BFS traversal
#include "fto.hpp"
#include "coordinate_fto.hpp"

static MoveTable<CORNER_CARD, NMOVES> cmt;
static MoveTable<EDGE_COMB_CARD, NMOVES> emt;
static MoveTable<TRIANGLE_CARD, NMOVES> tmt;
std::array<unsigned, CORNER_CARD> corner_z_shift_table; // 1 to 1 mapping between a corner state index and its conjugation through a z move

fs::path mtable_dir = "move_tables";
fs::path corner_mtable_path = mtable_dir / "corners";
fs::path edge_mtable_path = mtable_dir / "edges";
fs::path triangle_mtable_path = mtable_dir / "triangles";
fs::path edge_conversion_table_path = mtable_dir / "edge_conversion";
fs::path corner_z_shift_table_path = mtable_dir / "corner_z_shift";

bool load_move_tables() {
    if (cmt.load(corner_mtable_path)
        && emt.load(edge_mtable_path)
        && tmt.load(triangle_mtable_path)
        && load_table<CORNER_CARD>(corner_z_shift_table.data(), corner_z_shift_table_path)) return true;
    print("Move tables missing, generate first");
    return false;
}

void generate_corner_z_shift_table() {
    corner_z_shift_table.fill(CORNER_CARD);
    auto check_z_shift = [](const auto node) {
        FTO cube;
        for (auto m : node->template get_path<Move>()) {
            cube.apply(zSHIFT[m]);
        }
        corner_z_shift_table[corner_index(node->state)] = corner_index(cube);
    };

    auto is_treated = [](const auto node) -> bool {
        return (corner_z_shift_table[corner_index(node->state)] < CORNER_CARD);
    };

    BFS_traversal<FTO>(check_z_shift, is_treated, moves);
    write_table<CORNER_CARD>(corner_z_shift_table.data(), corner_z_shift_table_path);
}

// Only consider the combination positions
// of edges relative to their home RLBD face
unsigned edge_index(const EdgeComb& ecomb){
    return ecomb.pieces.index();
}

void edges_from_index(const unsigned &c, EdgeComb &ecomb) {
    ecomb.pieces.set_from_index(c);
}

void generate_move_tables() {
    if (!cmt.load(corner_mtable_path)) {
        cmt.compute<CubieFTO>(corner_index, corners_from_index, moves);
        cmt.write(corner_mtable_path);
    }

    if (!tmt.load(triangle_mtable_path)) {
        tmt.compute<CubieFTO>(tri1_index, tri1_from_index, moves);    
        tmt.write(triangle_mtable_path);
    }
    
    if (!emt.load(edge_mtable_path)) {
        emt.compute<EdgeComb>(edge_index, edges_from_index, moves);
        emt.write(edge_mtable_path);
    }

    if(!load_table<CORNER_CARD>(corner_z_shift_table.data(), corner_z_shift_table_path)) {
        generate_corner_z_shift_table();
    }

    assert(cmt.is_filled());
    assert(emt.is_filled());
    assert(tmt.is_filled());
}


FTO::FTO(const CubieFTO& cfto){
    static const Permutation<NE, true> z_edge{4,0,9,1,3,6,2,8,10,5,11,7}; // z'
    cp = corner_index(cfto);
    e1 = edge_comb_index(cfto.ep);
    e2 = edge_comb_index(cfto.ep.get_conjugate(z_edge));
    tri1 = tri1_index(cfto);
    tri2 = tri2_index(cfto);
}


void FTO::apply(const Move &m) {
    cmt.apply(m, cp);
    emt.apply(m, e1);
    emt.apply(zSHIFT[m], e2);
    tmt.apply(m, tri1);
    tmt.apply(zSHIFT[m], tri2);
};

void FTO::apply(const Sequence<Move> &seq) {
    for (auto m : seq) {
        apply(m);
    }
};

void FTO::show() const {
    print("Coordinate-level FTO object:");
    print("  cp =", cp);
    print("  e1 =", e1);
    print("  e2 =", e2);
    print("  tri1 =", tri1);
    print("  tri2 =", tri2);
}


unsigned corner_index(const FTO& fto){return fto.cp;}
void corners_from_index(const unsigned &c, FTO& fto){
    fto.cp = c;
};

// unsigned edge_index(const FTO& fto){
//     return fto.e1 * SIX_EDGE_PERM_CARD + (fto.e2 % SIX_EDGE_PERM_CARD);
// }

// void edges_from_index(const unsigned &c, FTO& fto){
//     unsigned e1 = c / SIX_EDGE_PERM_CARD;
//     unsigned cl = e1 / SIX_EDGE_PERM_CARD;
//     unsigned c1 = e1 % SIX_EDGE_PERM_CARD;
//     unsigned c2 = c % SIX_EDGE_PERM_CARD;

//     fto.e1 = cl * SIX_EDGE_PERM_CARD + c1;
//     fto.e2 = cl * SIX_EDGE_PERM_CARD + c2;
// }

unsigned tri1_index(const FTO& fto){return fto.tri1;}
void tri1_from_index(const unsigned &c, FTO& fto){
    fto.tri1 = c;
}

unsigned tri2_index(const FTO& fto){return fto.tri2;}
void tri2_from_index(const unsigned &c, FTO& fto){
    fto.tri2 = c;
}

unsigned triplet_index(const FTO& fto) {
    // Corners X triangles of first tetrad
    return fto.tri1 * CORNER_CARD + fto.cp;
}

void from_triplet_index(const unsigned &index, FTO& fto) {
    fto.tri1 = index / CORNER_CARD;
    fto.cp = index % CORNER_CARD;
}

unsigned triplet2_index(const FTO& fto) {
    // Corners X triangles of second tetrad
    return fto.tri2 * CORNER_CARD + corner_z_shift_table[fto.cp];
}

bool is_solved(const FTO &fto) {
    return fto.cp == 0 &&
        fto.e1 == 0 &&
        fto.e2 == 0 &&
        fto.tri1 == 0 &&
        fto.tri2 == 0;
}