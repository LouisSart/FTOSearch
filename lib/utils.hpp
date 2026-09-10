#pragma once
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std::chrono;

inline void init_array(unsigned *l, unsigned size, unsigned value) {
    for (unsigned i = 0; i < size; ++i) {
        l[i] = value;
    }
}

template <typename T, std::size_t n, typename cast_t = T>
void print_array(const typename std::array<T, n> &a) {
    std::cout << "{";
    for (unsigned k : a) {
        std::cout << static_cast<cast_t>(k) << ",";
    }
    if (n > 0) std::cout << "\b";
    std::cout << "}" << std::endl;
}

template <typename T, typename cast_t = T>
void print_vector(const typename std::vector<T> &v) {
    std::cout << "{";
    for (auto k : v) {
        std::cout << static_cast<cast_t>(k) << ", ";
    }
    if (v.size() > 0) std::cout << "\b\b";
    std::cout << "}" << std::endl;
}

template <typename T, typename... Ts>
void print(const T &truc, const Ts &...reste) {
    if constexpr (sizeof...(Ts) == 0) {
        std::cout << truc << std::endl;
    } else {
        std::cout << truc << " ";
        print(reste...);
    }
}

void time_fn(const auto f, const unsigned N = 1){
    //Time exec of a function
    auto start = high_resolution_clock::now();

    for (unsigned k = 0; k < N; ++k){
        f();
    }
    // Record ending time
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>( stop - start);

    print("Time taken: ", duration.count(), " microseconds");
}

template<std::size_t N, typename entry_t>
void write_table(const entry_t *data, const fs::path& path) {
    fs::create_directories(path.parent_path());
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char *>(data),
    sizeof(entry_t) * N);
    file.close();
}

template<std::size_t N, typename entry_t>
bool load_table(entry_t *data, const fs::path& path) {
    if (fs::exists(path)) {
        std::ifstream istrm(path, std::ios::binary);
        istrm.read(reinterpret_cast<char *>(data),
                    sizeof(entry_t) * N);
        istrm.close();
        return true;
    } else {
        print("Table not found at:", path);
        return false;
    }
}