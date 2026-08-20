#include "move.hpp"

std::ostream& operator<< (std::ostream& out, const Move& m){
    static constexpr std::string notation[NMOVES]
        {"U", "U'", "R", "R'", "F", "F'", "L", "L'", "B", "B'", "bR", "bR'", "D", "D'", "bL", "bL'"};
    
    out << notation[m];

    return out;
}

