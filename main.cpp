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

    return 0;
}