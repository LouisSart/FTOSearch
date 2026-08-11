#include "utils.hpp"
#include "move.hpp"
#include<cassert>


int main(int argc, const char* argv[]) {
    Permutation<NC> q;
    
    
    for (auto m : {U, R, F, L, B, bR, D, bL}){
        q.compose(CP[m]);
        q.compose(CP[m]);
        q.compose(CP[m]);
        assert(q.is_solved());
    }
    
    Permutation<NE> r;
    for (auto m : {U, R, F, L, B, bR, D, bL}){
        r.compose(EP[m]);
        r.compose(EP[m]);
        r.compose(EP[m]);
        assert(r.is_solved());
    }

    for (auto m : {R, U, R, U, R, U, R, U, R, U}){
        r.compose(EP[m]);
    }
    assert(r.is_solved());
    return 0;
}