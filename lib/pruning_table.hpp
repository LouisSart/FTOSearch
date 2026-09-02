#pragma once
#include <array>
#include <vector>
#include <algorithm>   // std::fill(begin, end)
#include <cassert>
#include <cstdint>     // uint8_t
#include <deque>       // std::deque
#include <filesystem>  // locate table files
#include <fstream>     // write tables into files
#include <limits>      // std::numeric_limits
#include <memory>      // std::shared_ptr
#include "fto.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

template <std::size_t N>
struct PruningTable {
    using entry_type = uint8_t;  // Cannot be printed, max representable
    // value of uint8_t is 255
    static constexpr unsigned unassigned =
        std::numeric_limits<entry_type>::max();

    std::shared_ptr<entry_type[]> table{new entry_type[N]};

    PruningTable() { std::fill(table.get(), table.get() + N, unassigned); }

    unsigned estimate(const unsigned &index) const {
        assert(index < N);
        return table[index];
    }

    void set(const unsigned &index, const unsigned &value) {
        assert(index < N);
        assert(value < unassigned);  // Ensure value fits in uint8_t
        table[index] = static_cast<entry_type>(value);
    }

    void reset() { std::fill(table.get(), table.get() + N, unassigned); }

    auto get_estimator() const {
        return [this](const unsigned &index) { return this->estimate(index); };
    }

    unsigned size() const { return N; }

    entry_type &operator[](const unsigned k) const {
        assert(k < N);
        return table[k];
    }

    bool is_assigned(const unsigned &index) const {
        assert(index < N);
        return table[index] != unassigned;
    }

    bool is_filled() const {
        // Check if all entries are assigned
        return std::all_of(table.get(), table.get() + N,
                           [](const entry_type &e) { return e != unassigned; });
    }

    unsigned DFS_fill(const auto &cube, const unsigned &depth,
                      const unsigned &fill_depth,
                      const auto &index, const auto &moves) {
        unsigned ret = 0;
        if (depth == fill_depth) {
            unsigned i = index(cube);
            if (!is_assigned(i)) {
                table[i] = fill_depth;
                ret = 1;
            }
        } else {
            for (const auto move : moves) {
                auto child = cube;
                child.apply(move);
                ret += DFS_fill(child, depth + 1, fill_depth, index,
                                moves);
            }
        }
        return ret;
    }

    template <typename Cube, bool verbose = false>
    void generate(unsigned (*index)(const Cube&),
                  void (*from_index)(const unsigned&, Cube&),
                  const auto &moves,
                  const unsigned forward_switch_depth = 3,
                  const unsigned backwards_switch_depth = 7) {
        assert(forward_switch_depth < backwards_switch_depth);

        std::vector<unsigned> distribution;
        unsigned node_counter = 0, nodes;
        unsigned fill_depth = 0;
        Cube cube;
        while (fill_depth < forward_switch_depth) {
            // IDDFS is only fast on the first few layers of the tree.
            nodes = DFS_fill(cube, 0, fill_depth, index, moves);

            if constexpr (verbose) print(fill_depth, nodes);
            node_counter += nodes;
            ++fill_depth;
            distribution.push_back(nodes);
        }

        // bool keep_forward = true; // CHECKME: Is that useful ?
        if constexpr (verbose) print("switch to forwards scan");
        while (fill_depth < backwards_switch_depth) {
            // Forward scan is efficient up to the level with most nodes
            nodes = 0;
            for (unsigned k = 0; k < N; ++k) {
                if (table[k] + 1 == fill_depth) {
                    from_index(k, cube);
                    for (const auto &m : moves) {
                        auto child = cube;
                        child.apply(m);

                        unsigned ci = index(child);
                        if (!is_assigned(ci)) {
                            table[ci] = fill_depth;
                            ++nodes;
                        }
                    }
                }
            }
            if constexpr (verbose) print(fill_depth, nodes);
            node_counter += nodes;

            assert(distribution.size() > 0);
            distribution.push_back(nodes);
            ++fill_depth;
        }

        if constexpr (verbose) print("switch to backwards scan");
        // Backwards scan from there
        while (node_counter < N) {
            nodes = 0;
            for (unsigned k = 0; k < N; ++k) {
                if (!is_assigned(k)) {
                    from_index(k, cube);
                    for (const auto &m : moves) {
                        auto parent = cube;
                        parent.apply(m);

                        unsigned pi = index(parent);
                        if ((unsigned)(table[pi] + 1) == fill_depth) {
                            table[k] = fill_depth;
                            ++nodes;
                            break;
                        }
                    }
                }
            }
            if constexpr (verbose) print(fill_depth, nodes);
            node_counter += nodes;
            distribution.push_back(nodes);
            ++fill_depth;
        }
        assert(is_filled());
        assert(N == node_counter);
    }

    std::vector<unsigned> get_distribution() const {
        std::vector<unsigned> ret = {0};
        unsigned h;
        for (auto it = table.get(); it < table.get() + N; ++it) {
            h = (unsigned)(*it);
            if (h + 1 > ret.size()) {
                ret.resize(h + 1);
            }
            ++ret[h];
        }
        return ret;
    }

    void show_distribution() {
        auto distr = get_distribution();
        float mean = 0.0;

        for (unsigned k = 0; k < distr.size(); ++k) {
            std::cout << std::setw(2) << k << " " << distr[k] << std::endl;
            mean += k * distr[k];
        }
        std::cout << "Mean value: " << mean / N << std::endl;
    }

    bool load(const fs::path &table_path) {
        if (fs::exists(table_path)) {
            std::ifstream istrm(table_path, std::ios::binary);
            istrm.read(reinterpret_cast<char *>(table.get()),
                       sizeof(entry_type) * N);
            istrm.close();
            assert(is_filled());
            return true;
        } else {
            print("Pruning table not found at: ", table_path);
        }
        return false;
    }

    void write(const fs::path &table_path) const {
        fs::create_directories(table_path.parent_path());
        std::ofstream file(table_path, std::ios::binary);
        file.write(reinterpret_cast<const char *>(table.get()),
                   sizeof(entry_type) * N);
        file.close();
    }
};