#pragma once
#include <algorithm>
#include <cassert>
#include <deque>

#include "../lib/node.hpp"

template <typename Cube>
struct Solutions : public std::vector<typename Node<Cube>::sptr> {
    unsigned best_hope;

    void sort_by_depth() {
        std::sort(this->begin(), this->end(),
                  [](const Node<Cube>::sptr node1, const Node<Cube>::sptr node2) {
                      return (node1->depth < node2->depth);
                  });
    }

    template<typename Move>
    void show() const {
        for (auto node : *this) {
            node->template get_path<Move>().show();
        }
    }
};

template <bool verbose = false, typename Cube>
Solutions<Cube> depth_first_search(std::deque<typename Node<Cube>::sptr> queue,
                                      unsigned (*estimate)(const Cube&),
                                      bool (*is_solved)(const Cube&),
                                      const auto &directions,
                                      const unsigned max_depth = 4) {
    // Main implementation starting from any number of root states
    Solutions<Cube> solutions;
    solutions.best_hope = 100;
    unsigned node_counter = 0, hope;

    while (queue.size() > 0) {
        auto node = queue.back();
        ++node_counter;
        if (is_solved(node->state)) {
            solutions.push_back(node);
            queue.pop_back();
        } else {
            queue.pop_back();
            hope = node->depth + estimate(node->state);
            if (hope <= max_depth) {
                auto children = node->expand(directions(node));
                for (auto &&child : children) {
                    queue.push_back(child);
                }
            } else {
                solutions.best_hope = std::min(solutions.best_hope, hope);
            }
        }
    }
    if constexpr (verbose) {
        std::cout << "Nodes generated: " << node_counter << std::endl;
    }
    return solutions;
}

template <bool verbose = false, typename Cube>
Solutions<Cube> depth_first_search(const typename Node<Cube>::sptr root,
                                      unsigned (*estimate)(const Cube&),
                                      bool (*is_solved)(const Cube&),
                                      const auto &directions,
                                      const unsigned max_depth = 4) {
    // Overload for solving a single starting position
    std::deque<typename Node<Cube>::sptr> queue({root});
    return depth_first_search<verbose>(queue, estimate, is_solved,
                                       directions, max_depth);
}

template <bool verbose = false, typename Cube>
Solutions<Cube> IDAstar(std::deque<typename Node<Cube>::sptr> roots,
                           unsigned (*estimate)(const Cube&), bool (*is_solved)(const Cube&),
                           const auto &directions,
                           const unsigned max_depth = 20,
                           const unsigned slackness = 0) {
    // Main implementation, starting from any number of root nodes
    unsigned search_depth = 100;
    for (auto root : roots) {
        search_depth =
            std::min(estimate(root->state) + root->depth, search_depth);
    }

    Solutions<Cube> solutions;
    while (solutions.size() == 0 && search_depth <= max_depth) {
        if constexpr (verbose) {
            std::cout << "Searching at depth " << search_depth << std::endl;
        }
        solutions = depth_first_search<verbose>(
            roots, estimate, is_solved, directions, search_depth);
        search_depth = solutions.best_hope;
    }
    if constexpr (verbose) {
        if (solutions.size() > 0) std::cout << "Solutions found" << std::endl;
    }
    if (slackness > 0) {
        // Find suboptimal with up to `slackness` extra moves
        // This implies that optimal solutions have been found
        // hence the job for those solutions is done twice.
        // I don' think this can be avoided since we need to
        // know optimal to introduce slackness
        search_depth = (max_depth < search_depth + slackness - 1)
                           ? max_depth
                           : search_depth + slackness - 1;
        if (verbose)
            std::cout << "Searching at depth " << search_depth << std::endl;
        solutions = depth_first_search<verbose>(
            roots, estimate, is_solved, directions, search_depth);
    }
    if constexpr (verbose) {
        if (solutions.size() == 0) {
            std::cout << "IDA*: No solution found" << std::endl;
        }
    }
    return solutions;
}

template <bool verbose = false, typename Cube>
Solutions<Cube> IDAstar(const typename Node<Cube>::sptr root,
                           unsigned (*estimate)(const Cube&), bool (*is_solved)(const Cube&),
                           const auto &directions,
                           const unsigned max_depth = 20,
                           const unsigned slackness = 0) {
    // Overload for solving a single starting position
    std::deque<typename Node<Cube>::sptr> queue{root};
    return IDAstar<verbose>(queue, estimate, is_solved, directions,
                            max_depth, slackness);
}