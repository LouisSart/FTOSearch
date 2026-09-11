#include <cstdlib>
#include <ctime>
#include "move_table.hpp"
#include "../lib/permutation.hpp"
#include "../lib/search.hpp" // BFS traversal
#include "fto.hpp"
#include "coordinate_fto.hpp"

static MoveTable<CORNER_CARD, NMOVES> cmt;
static MoveTable<SIX_EDGE_CARD, NMOVES> emt1;
static MoveTable<SIX_EDGE_CARD, NMOVES> emt2;
static MoveTable<TRIANGLE_CARD, NMOVES> tmt;
std::array<unsigned, CORNER_CARD> corner_z_shift_table; // 1 to 1 mapping between a corner state index and its conjugation through a z move

fs::path mtable_dir = "move_tables";
fs::path corner_mtable_path = mtable_dir / "corners";
fs::path edge_mtable_path_1 = mtable_dir / "edges1";
fs::path edge_mtable_path_2 = mtable_dir / "edges2";
fs::path triangle_mtable_path = mtable_dir / "triangles";
fs::path edge_conversion_table_path = mtable_dir / "edge_conversion";
fs::path corner_z_shift_table_path = mtable_dir / "corner_z_shift";

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
    return cl * SIX_EDGE_PERM_CARD + c2;
}

void e2_from_index(const unsigned &c, CubieFTO &fto) {
    fto.ep.set_from_split_indices(c / SIX_EDGE_PERM_CARD, 0, c % SIX_EDGE_PERM_CARD);
}

// Mapping between sparse index disregarding parity 
// and dense index for edge even permutation
static std::array<unsigned, EDGE_CARD * 2> edge_conversion;
void generate_edge_convert_table() {
    // Build a conversion table to retrieve the global 
    // permutation index from the split indices e1 and e2.
    Permutation<12, true> edges;
    edge_conversion.fill(EDGE_CARD);
    for (unsigned c = 0; c < EDGE_CARD; ++c){
        edges.set_from_index(c);
        auto [cl, c1, c2] = edges.split_indices();

        unsigned e1 = cl * SIX_EDGE_PERM_CARD + c1;
        unsigned sparse_idx = e1 * SIX_EDGE_PERM_CARD + c2;
        assert(sparse_idx < EDGE_CARD * 2);
        edge_conversion[sparse_idx] = c;
    }
}

void write_edge_convert_table() {
    write_table<EDGE_CARD * 2>(edge_conversion.data(), edge_conversion_table_path);
}

bool load_edge_convert_table() {
    return load_table<EDGE_CARD * 2>(edge_conversion.data(), edge_conversion_table_path);
}

unsigned dense_edge_index(const FTO& fto){
    return edge_conversion[edge_index(fto)];
}

void edges_from_dense_index(const unsigned &c, FTO& fto) {
    static CubieFTO cfto;
    cfto.ep.set_from_index(c);
    fto.e1 = e1_index(cfto);
    fto.e2 = e2_index(cfto);
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

void generate_move_tables() {
    if (!cmt.load(corner_mtable_path)) {
        cmt.compute<CubieFTO>(corner_index, corners_from_index, moves);
        cmt.write(corner_mtable_path);
    }

    if (!tmt.load(triangle_mtable_path)) {
        tmt.compute<CubieFTO>(tri1_index, tri1_from_index, moves);    
        tmt.write(triangle_mtable_path);
    }
    
    if (!emt1.load(edge_mtable_path_1)) {
        emt1.compute<CubieFTO>(e1_index, e1_from_index, moves);
        emt1.write(edge_mtable_path_1);
    }

    if (!emt2.load(edge_mtable_path_2)) {
        emt2.compute<CubieFTO>(e2_index, e2_from_index, moves);
        emt2.write(edge_mtable_path_2);
    }

    if(!load_table<CORNER_CARD>(corner_z_shift_table.data(), corner_z_shift_table_path)) {
        generate_corner_z_shift_table();
    }
}

FTO::FTO(const CubieFTO& cfto){
    cp = corner_index(cfto);
    e1 = e1_index(cfto);
    e2 = e2_index(cfto);
    tri1 = tri1_index(cfto);
    tri2 = tri2_index(cfto);
}


void FTO::apply(const Move &m) {
    cmt.apply(m, cp);
    emt1.apply(m, e1);
    emt2.apply(m, e2);
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

unsigned edge_index(const FTO& fto){
    return fto.e1 * SIX_EDGE_PERM_CARD + (fto.e2 % SIX_EDGE_PERM_CARD);
}

void edges_from_index(const unsigned &c, FTO& fto){
    unsigned e1 = c / SIX_EDGE_PERM_CARD;
    unsigned cl = e1 / SIX_EDGE_PERM_CARD;
    unsigned c1 = e1 % SIX_EDGE_PERM_CARD;
    unsigned c2 = c % SIX_EDGE_PERM_CARD;

    fto.e1 = cl * SIX_EDGE_PERM_CARD + c1;
    fto.e2 = cl * SIX_EDGE_PERM_CARD + c2;
}
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