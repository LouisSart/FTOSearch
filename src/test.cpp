#include "fto.hpp"
#include "coordinate_fto.hpp"
#include "solve.hpp"
#include "../lib/node.hpp"
#include "../lib/move_table.hpp"

#include<cassert>

int main(int argc, const char* argv[]) {

    // Test if cubie-level permutations are defined correctly
    CubieFTO fto;
    assert(fto.is_solved());

    for (auto m : {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2}){
        fto.apply({m, m, m});
        assert(fto.cp.is_solved());
        assert(fto.co.is_solved());
        assert(fto.tri1.is_solved());
    }

    fto.corner_apply({R, U, R, U, R, U, R, U, R, U, R, U});
    fto.corner_apply({R, F, R, F, R, F, R, F, R, F, R, F});
    fto.corner_apply({R, bR, R, bR, R, bR, R, bR, R, bR, R, bR});
    fto.corner_apply({B, bR, B, bR, B, bR, B, bR, B, bR, B, bR});
    fto.corner_apply({D, bR, D, bR, D, bR, D, bR, D, bR, D, bR});
    fto.corner_apply({D, F, D, F, D, F, D, F, D, F, D, F});
    fto.corner_apply({D, bR, D, bR, D, bR, D, bR, D, bR, D, bR});
    fto.corner_apply({U, L, U, L, U, L, U, L, U, L, U, L});
    fto.corner_apply({U, B, U, B, U, B, U, B, U, B, U, B});
    fto.corner_apply({bL, B, bL, B, bL, B, bL, B, bL, B, bL, B});
    fto.corner_apply({bL, L, bL, L, bL, L, bL, L, bL, L, bL, L});
    assert(fto.cp.is_solved());
    assert(fto.co.is_solved());

    fto.edge_apply({R, U, R, U, R, U, R, U, R, U});
    fto.edge_apply({R, F, R, F, R, F, R, F, R, F});
    fto.edge_apply({R, bR, R, bR, R, bR, R, bR, R, bR});
    fto.edge_apply({B, bR, B, bR, B, bR, B, bR, B, bR});
    fto.edge_apply({D, bR, D, bR, D, bR, D, bR, D, bR});
    fto.edge_apply({D, F, D, F, D, F, D, F, D, F});
    fto.edge_apply({D, bR, D, bR, D, bR, D, bR, D, bR});
    fto.edge_apply({U, L, U, L, U, L, U, L, U, L});
    fto.edge_apply({U, B, U, B, U, B, U, B, U, B});
    fto.edge_apply({bL, B, bL, B, bL, B, bL, B, bL, B});
    fto.edge_apply({bL, L, bL, L, bL, L, bL, L, bL, L});
    assert(fto.ep.is_solved());

    Sequence<Move> seq {U, R, F, L, B, bR, D, bL, bL2, D2, bR2, B2, L2, F2, R2, U2};
    fto.apply(seq);
    assert(fto.is_solved());

    assert(order({R, U, L, F}) == order({bL, B, bR, D}));

    // Test if indexing is correct
    Permutation<6> p;
    unsigned psize = factorial(3);
    for(unsigned c = 0; c < p.cardinality(); ++c) { // Any parity
        p.set_from_index(c);
        assert(p.index() == c);

        auto [cl, p1, p2] = p.split_indices();
        unsigned e = (cl * psize + p1) * psize + p2;
        // assert(e < p.cardinality() - 1); // this fails (good !)
        assert(e < p.cardinality()); // this doesn't (perfect !)

        p.set_from_split_indices(cl, p1, p2);
        assert(p.index() == c);
    }
    Permutation<6, true> q;
    unsigned psize1 = factorial(3);
    unsigned psize2 = factorial(3) / 2;
    for(unsigned c = 0; c < q.cardinality(); ++c) { // Even parity only
        q.set_from_index(c);
        assert(q.index() == c);

        auto [cl, p1, p2] = q.split_indices();
        unsigned e = (cl * psize1 + p1) * psize2 + p2;
        // assert(e < q.cardinality() - 1); // this fails (good !)
        assert(e < q.cardinality()); // this doesn't (perfect !)

        q.set_from_split_indices(cl, p1, p2);
        auto [_cl, _p1, _p2] = q.split_indices();
        assert(_cl == cl && _p1 == p1 && _p2 == p2);
        assert(q.index() == c);
    }
    unsigned card = Layout<11, 4>::CARD * Permutation<4>::CARD;
    Permutation<11> r;
    for(unsigned c = 0; c < card; ++c) { // Any parity
        r.set_from_partial_index<4>(c, {1,3,5,7});
        assert(r.partial_index<4>({1,3,5,7}) == c);
    }
    Orientation<7> o; // even orientations
    for (unsigned k = 0; k < o.cardinality(); ++k) {
        o.set_from_index(k);
        assert(o.index() == k);
    }
    Orientation<6, false> O; // any orientation
    for (unsigned k = 0; k < O.cardinality(); ++k) {
        O.set_from_index(k);
        assert(O.index() == k);
    }
    Layout<10, 3> l;
    for (unsigned c = 0; c < l.cardinality(); ++c){
        l.set_from_index(c);
        assert(l.index() == c);
    }
    Center<12, 4> center;
    for (unsigned c = 0; c < center.cardinality(); ++c){
        center.set_from_index(c);
        assert(center.index() == c);
    }

    // Some testing on sequences and moves
    auto node = make_root(CubieFTO(), true);
    for (unsigned k = 0; k < NMOVES; ++k) {
        node = node->expand(moves)[k];
    }
    node->get_path<Move>();
    auto root = node->get_root();
    allowed_next(bL);

    MoveTable<CORNER_CARD, NMOVES> cmt;
    cmt.compute<CubieFTO>(corner_index, corners_from_index, moves);

    if (!load_move_tables()) {
        generate_move_tables();
    }
    generate_corner_table();

    FTO cube;
    for (auto m : seq) {
        cube.apply(m);
    }
    assert(cube.cp == 0);
    assert(cube.e1 == 0);
    assert(cube.e2 == 0);
    assert(cube.tri1 == 0);
    assert(cube.tri2 == 0);

    return 0;
}