#include "fto.hpp"
#include "table.hpp"
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

    Sequence seq {U, R, F, L, B, bR, D, bL, bL2, D2, bR2, B2, L2, F2, R2, U2};
    fto.apply(seq);
    assert(fto.is_solved());

    assert(order({R, U, L, F}) == order({bL, B, bR, D}));

    // Test if indexing is correct
    Permutation<6> p;
    for(unsigned c = 0; c < 720; ++c) { // Any parity
        p.set_from_index(c);
        assert(p.index() == c);
    }
    for(unsigned c = 0; c < 360; ++c) { // Even parity only
        p.set_from_index(c, true);
        assert(p.index(true) == c);
    }
    Orientation<6> o;
    for (unsigned k = 0; k < 32; ++k) {
        o.set_from_index(k);
        assert(o.index() == k);
    }

    generate_corner_table();

    Center<12, 4> center;
    for (unsigned c = 0; c < 369600; ++c){
        center.from_index(c);
        assert(center.index() == c);
    }

    return 0;
}