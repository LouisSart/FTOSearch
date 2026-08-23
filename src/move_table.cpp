#include "move_table.hpp"
#include "permutation.hpp"
#include "fto.hpp"

constexpr unsigned CP_CARD = Permutation<6, true>::CARD;
constexpr unsigned CO_CARD = Orientation<6>::CARD;
constexpr unsigned CORNER_CARD = CP_CARD * CO_CARD;
constexpr unsigned EDGE_CARD = Permutation<12, true>::CARD;
constexpr unsigned TRIANGLE_CARD = Center<12, 4>::CARD;

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
    cmt.compute<CubieFTO, true>(corner_index, corners_from_index, moves);
    cmt.write(corner_mtable_path);

    tmt.compute<CubieFTO, true>(tri1_index, tri1_from_index, moves);
    tmt.write(triangle_mtable_path);

    emt.compute<CubieFTO, true>(edge_index, edges_from_index, moves);
    emt.write(edge_mtable_path);

}

void apply(const Move &m, FTO &fto){
    cmt.apply(m, fto.cp);
    emt.apply(m, fto.ep);
    tmt.apply(m, fto.tri1);
    tmt.apply(zSHIFT[m], fto.tri2);
};