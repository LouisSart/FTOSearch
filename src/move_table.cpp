#include "move_table.hpp"
#include "permutation.hpp"
#include "fto.hpp"

static constexpr unsigned FIVE_EDGE_CARD = Layout<NE, 5>::CARD * Permutation<5>::CARD;
static MoveTable<CORNER_CARD, NMOVES> cmt;
static MoveTable<FIVE_EDGE_CARD, NMOVES> emt1;
static MoveTable<FIVE_EDGE_CARD, NMOVES> emt2;
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
    return false;
}


// Split edges into two parts otherwise the move table is 15 GB lool
unsigned e1_index(const CubieFTO& fto){
    return fto.ep.partial_index<5>({0, 1, 2, 3, 4});
}

void e1_from_index(const unsigned &c, CubieFTO &fto) {
    fto.ep.set_from_partial_index<5>(c, {0, 1, 2, 3, 4});
}

unsigned e2_index(const CubieFTO& fto){
    return fto.ep.partial_index<5>({5, 6, 7, 8, 9});
}

void e2_from_index(const unsigned &c, CubieFTO &fto) {
    fto.ep.set_from_partial_index<5>(c, {5, 6, 7, 8, 9});
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
