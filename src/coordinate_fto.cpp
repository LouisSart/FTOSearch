#include "move_table.hpp"
#include "../lib/permutation.hpp"
#include "fto.hpp"
#include "coordinate_fto.hpp"

static constexpr unsigned SIX_EDGE_PERM_CARD = Permutation<6>::CARD;
static constexpr unsigned SIX_EDGE_EVEN_PERM_CARD = Permutation<6>::CARD / 2;
static constexpr unsigned SIX_EDGE_CARD = Layout<NE, 6>::CARD * SIX_EDGE_PERM_CARD;
static constexpr unsigned SIX_EDGE_EVEN_CARD = Layout<NE, 6>::CARD * SIX_EDGE_EVEN_PERM_CARD;
static MoveTable<CORNER_CARD, NMOVES> cmt;
static MoveTable<SIX_EDGE_CARD, NMOVES> emt1;
static MoveTable<SIX_EDGE_EVEN_CARD, NMOVES> emt2;
static MoveTable<TRIANGLE_CARD, NMOVES> tmt;

fs::path mtable_dir = "move_tables";
fs::path corner_mtable_path = mtable_dir / "corners";
fs::path edge_mtable_path_1 = mtable_dir / "edges1";
fs::path edge_mtable_path_2 = mtable_dir / "edges2";
fs::path triangle_mtable_path = mtable_dir / "triangles";

bool load_move_tables() {
    if (cmt.load(corner_mtable_path)
        && emt1.load(edge_mtable_path_1)
        && emt2.load(edge_mtable_path_2)
        && tmt.load(triangle_mtable_path)) return true;
    print("Move tables missing, generate first");
    return false;
}


// Split edges into two parts otherwise the move table is 15 GB lool
// First part is a 6 edge partial permutation (any parity)
unsigned e1_index(const CubieFTO& fto){
    auto [cl, c1, c2] = fto.ep.split_indices();
    return cl * SIX_EDGE_PERM_CARD + c1;
}

void e1_from_index(const unsigned &c, CubieFTO &fto) {
    fto.ep.set_from_split_indices(c / SIX_EDGE_PERM_CARD, c % SIX_EDGE_PERM_CARD, 0);
}

// Second part takes care of the last 6 edges partial
// perm but ignores the actual permutation of the last 2
// because it is forced by the parity of the first set e1
unsigned e2_index(const CubieFTO& fto){
    auto [cl, c1, c2] = fto.ep.split_indices();
    return cl * SIX_EDGE_EVEN_PERM_CARD + c2;
}

void e2_from_index(const unsigned &c, CubieFTO &fto) {
    fto.ep.set_from_split_indices(c / SIX_EDGE_EVEN_PERM_CARD, 0, c % SIX_EDGE_EVEN_PERM_CARD);
}

void generate_move_tables() {
    cmt.compute<CubieFTO>(corner_index, corners_from_index, moves);
    cmt.write(corner_mtable_path);

    tmt.compute<CubieFTO>(tri1_index, tri1_from_index, moves);
    tmt.write(triangle_mtable_path);

    emt1.compute<CubieFTO>(e1_index, e1_from_index, moves);
    emt1.write(edge_mtable_path_1);

    emt2.compute<CubieFTO>(e2_index, e2_from_index, moves);
    emt2.write(edge_mtable_path_2);
}

void FTO::apply(const Move &m) {
    cmt.apply(m, cp);
    emt1.apply(m, e1);
    emt2.apply(m, e2);
    tmt.apply(m, tri1);
    tmt.apply(zSHIFT[m], tri2);
};

unsigned corner_index(const FTO& fto){return fto.cp;}
void corners_from_index(const unsigned &c, FTO& fto){
    fto.cp = c;
};

unsigned edge_index(const FTO& fto){
    return fto.e1 * SIX_EDGE_EVEN_PERM_CARD + (fto.e2 % SIX_EDGE_EVEN_PERM_CARD);
}

void edges_from_index(const unsigned &c, FTO& fto){
    unsigned e1 = c / SIX_EDGE_EVEN_PERM_CARD;
    unsigned cl = e1 / SIX_EDGE_PERM_CARD;
    unsigned c1 = e1 % SIX_EDGE_PERM_CARD;
    unsigned c2 = c % SIX_EDGE_EVEN_PERM_CARD;

    fto.e1 = cl * SIX_EDGE_PERM_CARD + c1;
    fto.e2 = cl * SIX_EDGE_EVEN_PERM_CARD + c2;
}
unsigned tri1_index(const FTO& fto){return fto.tri1;}
void tri1_from_index(const unsigned &c, FTO& fto){
    fto.tri1 = c;
};