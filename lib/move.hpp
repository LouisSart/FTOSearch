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