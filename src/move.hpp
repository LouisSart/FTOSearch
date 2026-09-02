#pragma once
#include "../lib/move.hpp"

// Declare in puzzle def files
enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
std::ostream& operator<< (std::ostream& out, const Move& m);
constexpr unsigned NMOVES = 16;
constexpr Move moves[NMOVES] = {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

// Reduce branching by preventing turning twice the same layer / 
// chaining opposite moves
const std::vector<Move> &allowed_next(const Move m);
