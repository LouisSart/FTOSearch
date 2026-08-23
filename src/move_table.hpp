#pragma once
#include <algorithm>   // std::fill, std::all_of
#include <filesystem>  // locate table files
#include <fstream>     // write tables into files
#include <memory>      // std::shared_ptr

#include "utils.hpp"

namespace fs = std::filesystem;

template <unsigned N, unsigned NMOVES>
struct MoveTable {
    std::shared_ptr<unsigned[]> table{new unsigned[N * NMOVES]};

    fs::path move_table_dir() const { return "move_tables/"; }

    MoveTable() { std::fill(table.get(), table.get() + N, N); }

    template <typename Cube>
    void compute(const auto &index, const auto &from_index, const auto moves) {
        // index : a function that takes in a Cube instance and returns
        // some coordinate for it
        // from_index : a function that takes in a
        // coordinate value and returns a Cube instance for it

        for (unsigned c = 0; c < N; ++c) {
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

    bool load(fs::path filename) {
        auto table_path = move_table_dir() / filename;
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

    void write(fs::path filename) const {
        auto table_path = move_table_dir() / filename;
        fs::create_directories(table_path.parent_path());
        std::ofstream file(table_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(table.get()),
                   sizeof(unsigned) * N * NMOVES);
        file.close();
    }
};

//SHOULD BE IN A SEPARATE FILE
#include "permutation.hpp"
constexpr unsigned CORNER_CARD = Permutation<6, true>::CARD * Orientation<6>::CARD;
constexpr unsigned EDGE_CARD = Permutation<12, true>::CARD;
constexpr unsigned TRIANGLE_CARD = Center<12, 4>::CARD;