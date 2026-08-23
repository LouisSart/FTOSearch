#include "move_table.hpp"
#include "permutation.hpp"
#include "fto.hpp"

static MoveTable<CORNER_CARD, NMOVES> cmt;
static MoveTable<EDGE_CARD, NMOVES> emt;
static MoveTable<TRIANGLE_CARD, NMOVES> tmt;

fs::path mtable_dir = "move_tables";
fs::path corner_mtable_path = mtable_dir / "corners";
fs::path edge_mtable_path = mtable_dir / "edges";
fs::path triangle_mtable_path = mtable_dir / "triangles";

void load_move_tables() {
    cmt.load(corner_mtable_path);
    emt.load(edge_mtable_path);
    tmt.load(triangle_mtable_path);
}

void generate_move_tables() {
    cmt.compute<CubieFTO>(corner_index, corners_from_index, moves);
    cmt.write(corner_mtable_path);

    tmt.compute<CubieFTO>(tri1_index, tri1_from_index, moves);
    tmt.write(triangle_mtable_path);

    emt.compute<CubieFTO>(edge_index, edges_from_index, moves);
    emt.write(edge_mtable_path);
}

void FTO::apply(const Move &m) {
    cmt.apply(m, cp);
    emt.apply(m, ep);
    tmt.apply(m, tri1);
    tmt.apply(zSHIFT[m], tri2);
};
