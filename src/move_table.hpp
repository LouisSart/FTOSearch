#pragma once
#include <algorithm>   // std::fill, std::all_of
#include <filesystem>  // locate table files
#include <fstream>     // write tables into files
#include <memory>      // std::shared_ptr
#include <cassert>     // assert
#include "utils.hpp"

namespace fs = std::filesystem;

template <unsigned N, unsigned NMOVES>
struct MoveTable {
    std::shared_ptr<unsigned[]> table{new unsigned[N * NMOVES]};

    MoveTable() { std::fill(table.get(), table.get() + N, N); }

    template <typename Cube, bool verbose = false>
    void compute(const auto &index, const auto &from_index, const auto moves) {
        // index : a function that takes in a Cube instance and returns
        // some coordinate for it
        // from_index : a function that takes in a
        // coordinate value and returns a Cube instance for it

        if constexpr (verbose) {
            print("Building move table of size", N, "for", NMOVES, "moves");
        }
        for (unsigned c = 0; c < N; ++c) {
            if constexpr (verbose) {
                if (c % (N / 10) == 0) print(c / (N / 10) * 10, "%");
            }
            Cube cc = from_index(c);
            for (unsigned m = 0; m < NMOVES; ++m) {
                auto buffer_cc = cc;
                buffer_cc.apply(moves[m]);
                table[c * NMOVES + m] = index(buffer_cc);
            }
        }
    }

    bool is_filled() const {
        // Check if all entries are assigned
        return std::all_of(table.get(), table.get() + N * NMOVES,
                           [](const unsigned &e) { return e < N; });
    }

    template<typename Move>
    void apply(const Move &m, unsigned &c) { c = table[c * NMOVES + m]; }

    bool load(fs::path table_path) {
        if (fs::exists(table_path)) {
            std::ifstream istrm(table_path, std::ios::binary);
            istrm.read(reinterpret_cast<char *>(table.get()),
                       sizeof(unsigned) * N * NMOVES);
            istrm.close();
            assert(is_filled());
            return true;
        } else {
            print("Move table not found at: ", table_path);
        }
        return false;
    }

    void write(fs::path table_path) const {
        fs::create_directories(table_path.parent_path());
        std::ofstream file(table_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(table.get()),
                   sizeof(unsigned) * N * NMOVES);
        file.close();
    }
};

#include "fto.hpp"
#include "move.hpp"

struct FTO {
    // Coordinate level representation of an FTO
    unsigned cp;   // Corner permutation + orientation
    unsigned ep;   // Edge permutation
    unsigned tri1; // Triangles of first tetrad
    unsigned tri2; // Triangles of second tetrad

    unsigned corner_index() const;
};

bool is_solved(const FTO &fto);
// unsigned corner_index(const FTO& fto);
// FTO corners_from_index(const unsigned &c);
// unsigned edge_index(const FTO& fto);
// FTO edges_from_index(const unsigned &c);
// unsigned tri1_index(const FTO& fto);
// FTO tri1_from_index(const unsigned &c);

void load_move_tables();
void generate_move_tables();
// void apply(const Move &m, FTO &fto);