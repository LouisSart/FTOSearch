#include "utils.hpp"
#include "move.hpp"
#include<cassert>


int main(int argc, const char* argv[]) {
    
    Permutation<NC> q;
    for (auto m : {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2}){
        corner_apply(q, {m, m, m});
        assert(q.is_solved());
    }

    corner_apply(q, {R, U, R, U, R, U, R, U, R, U, R, U});
    assert(q.is_solved());
    corner_apply(q, {R, F, R, F, R, F, R, F, R, F, R, F});
    assert(q.is_solved());
    corner_apply(q, {R, bR, R, bR, R, bR, R, bR, R, bR, R, bR});
    assert(q.is_solved());
    corner_apply(q, {B, bR, B, bR, B, bR, B, bR, B, bR, B, bR});
    assert(q.is_solved());
    corner_apply(q, {D, bR, D, bR, D, bR, D, bR, D, bR, D, bR});
    assert(q.is_solved());
    corner_apply(q, {D, F, D, F, D, F, D, F, D, F, D, F});
    assert(q.is_solved());
    corner_apply(q, {D, bR, D, bR, D, bR, D, bR, D, bR, D, bR});
    assert(q.is_solved());
    corner_apply(q, {U, L, U, L, U, L, U, L, U, L, U, L});
    assert(q.is_solved());
    corner_apply(q, {U, B, U, B, U, B, U, B, U, B, U, B});
    assert(q.is_solved());
    corner_apply(q, {bL, B, bL, B, bL, B, bL, B, bL, B, bL, B});
    assert(q.is_solved());
    corner_apply(q, {bL, L, bL, L, bL, L, bL, L, bL, L, bL, L});
    assert(q.is_solved());
    
    Permutation<NE> r;
    for (auto m : {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2}){
        edge_apply(r, {m, m, m});
        assert(r.is_solved());
    }

    edge_apply(r, {R, U, R, U, R, U, R, U, R, U});
    assert(r.is_solved());
    edge_apply(r, {R, F, R, F, R, F, R, F, R, F});
    assert(r.is_solved());
    edge_apply(r, {R, bR, R, bR, R, bR, R, bR, R, bR});
    assert(r.is_solved());
    edge_apply(r, {B, bR, B, bR, B, bR, B, bR, B, bR});
    assert(r.is_solved());
    edge_apply(r, {D, bR, D, bR, D, bR, D, bR, D, bR});
    assert(r.is_solved());
    edge_apply(r, {D, F, D, F, D, F, D, F, D, F});
    assert(r.is_solved());
    edge_apply(r, {D, bR, D, bR, D, bR, D, bR, D, bR});
    assert(r.is_solved());
    edge_apply(r, {U, L, U, L, U, L, U, L, U, L});
    assert(r.is_solved());
    edge_apply(r, {U, B, U, B, U, B, U, B, U, B});
    assert(r.is_solved());
    edge_apply(r, {bL, B, bL, B, bL, B, bL, B, bL, B});
    assert(r.is_solved());
    edge_apply(r, {bL, L, bL, L, bL, L, bL, L, bL, L});
    assert(r.is_solved());
    return 0;
}