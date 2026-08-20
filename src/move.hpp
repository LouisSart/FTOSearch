#pragma once
#include <vector>
#include <string>
#include <iostream>


// Define in lib header
template<typename Move>
struct Sequence : std::vector<Move> {
    bool inv_flag;

    Sequence() {};
    template<typename... Args>
    Sequence(Args... args) : std::vector<Move>{{ args... }} {} // Constructeur par brace-enclosed
    
    void show(const bool show_length=true, const bool line_break=true) const {
        if (inv_flag) std::cout << "(";
        for (const auto m : *this) {
            std::cout << m << " ";
        }
        if (this->size() > 0) std::cout << "\b";
        if (inv_flag) std::cout << ")";
        if (show_length) std::cout << " (" << this->size() << ")";
        if (line_break) std::cout << std::endl;
    }
    void prepend(const Move &m){
        this->insert(this->begin(), m);
    }
    void prepend(const unsigned &m){
        this->insert(this->begin(), static_cast<Move>(m));
    }
};

// Declare in puzzle def files
enum Move : unsigned {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};
std::ostream& operator<< (std::ostream& out, const Move& m);
constexpr unsigned NMOVES = 16;
constexpr Move moves[NMOVES] = {U, U2, R, R2, F, F2, L, L2, B, B2, bR, bR2, D, D2, bL, bL2};

// Reduce branching by preventing turning twice the same layer / 
// chaining opposite moves
const std::vector<Move> &allowed_next(const Move m);
