#pragma once
#include <vector>
#include <string>
#include <iostream>

constexpr unsigned NMOVES = 16;
enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

struct Sequence : std::vector<Move> {
    template<typename... Args>
    Sequence(Args... args) : std::vector<Move>{{ args... }} {} // Constructeur par brace-enclosed
    void show(const bool show_length=true, const bool line_break=true) const {
        static const std::string notation[NMOVES]
            {"U", "U'", "R", "R'", "F", "F'", "L", "L'", "B", "B'", "bR", "bR'", "D", "D'", "bL", "bL'"};
        
        for (const auto m : *this) {
            std::cout << notation[m] << " ";
        }
        
        if (show_length) std::cout << "(" << size() << ")";
        if (line_break) std::cout << std::endl;
    }
};
inline constexpr Move moves[NMOVES] = {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

