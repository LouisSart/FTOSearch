#include "move.hpp"

std::ostream& operator<< (std::ostream& out, const Move& m){
    static constexpr std::string notation[NMOVES]
        {"U", "U'", "R", "R'", "F", "F'", "L", "L'", "B", "B'", "bR", "bR'", "D", "D'", "bL", "bL'"};
    
    out << notation[m];

    return out;
}

const std::vector<Move> &allowed_next(const Move m) {
    static std::vector<Move> all {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterU {R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterD {R, R2, F, F2, L, L2, B, B2, bR, bR2, bL, bL2};
    static std::vector<Move> afterF {U, U2, R, R2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterB {U, U2, R, R2, L, L2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterR {U, U2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterbL {U, U2, F, F2, L, L2, B, B2, bR, bR2, D, D2};
    static std::vector<Move> afterL {U, U2, R, R2, F, F2, B, B2, bR, bR2, D, D2, bL, bL2};
    static std::vector<Move> afterbR {U, U2, R, R2, F, F2, B, B2, D, D2, bL, bL2};

    switch (m) {
        case U ... U2:
            return afterU;
        case D ... D2:
            return afterD;
        case R ... R2:
            return afterR;
        case L ... L2:
            return afterL;
        case F ... F2:
            return afterF;
        case B ... B2:
            return afterB;
        case bR ... bR2:
            return afterbR;
        case bL ... bL2:
            return afterbL;
        default:
            return all;
    }
}

