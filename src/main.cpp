#include "fto.hpp"
#include <cassert>
#include <set>
#include <cstdint>

static Sequence sequences[5] {{R, U}, {R, U2}, {R, U, L, F}, {R2, L, R, L2}, {R, B2, R2, D, L2}};

// auto corner_index(const CubieFTO& fto){
//     return fto.corner_index();
// }

std::array<uint8_t, 360 * 32> cp_table;
// void DFS_count(const CubieFTO &fto, auto index, std::set<unsigned> counted, unsigned depth, unsigned max_depth) {
//     if (depth == max_depth && ) {

//     }
// }
int main(int argc, const char* argv[]) {

    cp_table.fill(255);
    CubieFTO fto;
    cp_table[fto.corner_index()] = 0;
    unsigned visited = 1;
    print(0, visited);

    // Forward scan
    unsigned depth = 1;
    while (visited > 0){
        visited = 0;
        for (unsigned k = 0; k < 360 * 32; ++k){
            if (cp_table[k] == depth - 1){
                fto.set_corners_from_index(k);
                for (const Move &m : moves) {
                    CubieFTO child = fto;
                    child.apply(m);
                    unsigned c = child.corner_index(); 
                    if (cp_table[c] == 255) {
                        cp_table[c] = depth;
                        ++visited;
                    }
                }
            }
        }
        print(depth, visited);
        depth += 1;
    }
    return 0;
}