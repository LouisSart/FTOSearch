#pragma once
#include <array>
#include <set>
#include <cassert>
#include <algorithm>
#include "utils.hpp"

constexpr unsigned ipow(unsigned k, unsigned n) {
    // unsigned integer power
    // computes ret = k^n
    unsigned ret{1};
    for (unsigned i = 0; i < n; i++) {
        ret *= k;
    }
    return ret;
};

constexpr auto factorial_table = [] {
    std::array<unsigned, 13> arr = {};
    arr[0] = 1;
    for (unsigned n = 1; n <= 12; ++n) {
        arr[n] = n * arr[n - 1];
    }
    return arr;
}();

constexpr unsigned factorial(unsigned n) {
    assert(n <= 12);
    return factorial_table[n];
}

constexpr auto binomial_table = [] {
    // Using constexpr lambda to fill up binomial table
    // at compile time
    std::array<unsigned, (13) * (13)> arr = {};
    for (unsigned n = 0; n <= 12; ++n) {
        for (unsigned k = 0; k <= 12; ++k) {
            if (n < k) {
                arr[n * 13 + k] = 0;
            } else if (k == 0 || k == n) {
                arr[n * 13 + k] = 1;
            } else {
                arr[n * 13 + k] =
                    arr[(n - 1) * 13 + k - 1] + arr[(n - 1) * 13 + k];
            }
        }
    }
    return arr;
}();

constexpr unsigned binomial(unsigned n, unsigned k) {
    // The function just does a lookup in the table for better performance
    assert(n < 13 && k < 13);  // "Binomial numbers computed up to n=12"
    return binomial_table[n * 13 + k];
}


template <std::size_t n>
unsigned layout_index(const std::array<unsigned, n> &layout, unsigned r) {
    // n: number of positions
    // r: number of pieces
    assert(r <= n);
    assert(0 < n);
    unsigned t = 0;
    for (unsigned i = n - 1; i > 0; --i) {
        if (layout[i] == 1) {
            t += binomial(i, r);
            r -= 1;
        }
    }
    return t;
}

template <std::size_t n>
void layout_from_index(unsigned c, std::array<unsigned, n> &layout,
                       unsigned r) {
    // n: number of positions
    // r: number of pieces
    assert(r <= n);
    assert(c < binomial(n, r));
    for (auto i = n - 1; i < n; --i) {
        if (c >= binomial(i, r)) {
            c = c - binomial(i, r);
            layout[i] = 1;
            r = r - 1;
        } else {
            layout[i] = 0;
        }
    }
}

template <unsigned N, unsigned M>
struct Layout : std::array<unsigned, N> {
    // holds the positions of M elements amongst
    // N positions regardless of order
    static constexpr unsigned filled = 1;
    static constexpr unsigned empty = 0;

    Layout() { // Constructeur sans arguments
        static_assert(M < N);
        this->fill(empty);
        for (unsigned k = 0; k < M; ++k){
            this->operator[](k) = filled;
        }
    }

    Layout(const unsigned &c) { // Constructeur from index
        this->set_from_index(c);
    }

    Layout(const std::array<unsigned, N> &p, const std::array<unsigned, M> &pieces) {
        for (unsigned k = 0; k < N; ++k) {
            if (std::find(pieces.begin(), pieces.end(), p[k]) != pieces.end()){
                this->operator[](k) = 1;
            } else {
                this->operator[](k) = 0;
            }
        }
    }

    constexpr unsigned cardinality() const {
        return CARD;
    }
    static constexpr unsigned CARD = binomial(N, M);

    unsigned index() const {
        return layout_index(*this, M);
    }

    void set_from_index(const unsigned &c) {
        layout_from_index(c, *this, M);
    }
};

template<unsigned N, bool even = false>
struct Permutation : std::array<unsigned, N> {

    Permutation() { // Constructeur sans arguments
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }

    Permutation(const unsigned &c) { // Constructeur from index
        this->set_from_index(c);
    }

    template<typename... Args>
    Permutation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k){
            if (this->operator[](k) != k) return false;
        }
        return true;
    }

    static constexpr unsigned cardinality() {
        if constexpr (even) return factorial(N) / 2;
        return factorial(N);
    }
    static constexpr unsigned CARD = cardinality();

    void reset() {
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }
    void compose(const Permutation<N> & other) {
        auto ret = *this;
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = ret[other[k]];
        }
    }
    unsigned index() const {
        // Compute the lexicographic index of the permutation
        static_assert(N > 0); // empty permutations are a problem
        // If the permutation necessarily has even parity,
        // do not encode the information of the last two digits
        unsigned n;
        if constexpr (even) {n = N - 1;
            assert(N > 2); // trivial case do not implement
        }
        else n = N;
        
        unsigned t = 0;
        for (unsigned i = 0; i < n - 1; ++i){
            t = t * (N - i);
            for (unsigned j = i; j < N; ++j) {
                if ((*this)[i] > (*this)[j]) t = t + 1;
            }
        }
        return t;
    }
    void set_from_index(unsigned c) {
        // Reconstruct the permutation having index c
        static_assert(N > 0); // empty permutations are a problem

        if constexpr (even) {
            assert(N > 2); // trivial case do not implement
            unsigned s = 0;
            (*this)[N - 1] = 1;
            (*this)[N - 2] = 0;
            for (unsigned i = N - 3; i < N; --i) {
                (*this)[i] = (c % (N - i));
                s += (*this)[i];
                c = c / (N - i);
                for (auto j = i + 1; j < N; ++j) {
                    if ((*this)[j] >= (*this)[i]) {
                        (*this)[j] += 1;
                    }
                }
            }
            if (s % 2 == 1) swap(N - 1, N - 2);
        } else {
            (*this)[N - 1] = 0;
            for (unsigned i = N - 2; i < N; --i) {
                (*this)[i] = (c % (N - i));
                c = c / (N - i);
                for (auto j = i + 1; j < N; ++j) {
                    if ((*this)[j] >= (*this)[i]) {
                        (*this)[j] += 1;
                    }
                }
            }
        }
    }
    void swap(const unsigned &i, const unsigned &j) {
        assert(i < N);
        assert(j < N);
        unsigned buf = (*this)[i];
        (*this)[i] = (*this)[j];
        (*this)[j] = buf;
    }

    template<unsigned M>
    Permutation<M> get_sub_permutation() const {
        // returns the sub permutation for values
        // 0 to M - 1
        static_assert(M < N);
        Permutation<M> ret;
        auto it = ret.begin();
        for (unsigned k : *this) {
            if (k < M) {
                *it = k;
                ++it;
            }
        }
        return ret;
    }

    template<long unsigned M>
    Permutation<M> get_sub_permutation(const std::array<unsigned, M> &pieces) const {
        // returns the sub permutation of elements in
        // array pieces
        static_assert(M < N);
        Permutation<M> ret;
        unsigned i = 0;
        for (unsigned k : *this) {
            auto it = std::find(pieces.begin(), pieces.end(), k);
            if (it != pieces.end()) {
                ret[i] = it - pieces.begin();
                ++i;
            }
        }
        return ret;
    }

    template<long unsigned M>
    unsigned partial_index(const std::array<unsigned, M> &pieces) const {
        static_assert(M < N);
        Layout<N, M> l(*this, pieces);
        Permutation<M> p = get_sub_permutation(pieces);
        
        return l.index() * p.cardinality() + p.index();
    }
    
    template<long unsigned M>
    void set_from_partial_index(const unsigned &c, const std::array<unsigned, M> &pieces) {
        Permutation<M> p(c % Permutation<M>::CARD);
        Layout<N, M> l(c / Permutation<M>::CARD);

        unsigned k = 0;
        for (unsigned i = 0; i < N; ++i) {
            if (l[i] == 1) {
                (*this)[i] = pieces[p[k]];
                ++k;
            } else {
                (*this)[i] = N;
            }
        }
    }
};

template<unsigned N, unsigned v = 2, bool even = true>
struct Orientation : std::array<unsigned, N> {
    // array that stores the orientations of N pieces modulo v
    // if an Orientation is even, then the total
    // orientation of the pieces is 0 modulo v.
    // In this case the orientation of the last piece
    // is forced by the others.

    Orientation() { // Constructeur sans arguments
        std::array<unsigned, N>::fill(0);
    }
    template<typename... Args>
    Orientation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    static constexpr unsigned cardinality() {
        if constexpr (even) return ipow(v, N - 1);
        return ipow(v, N);
    }
    static constexpr unsigned CARD = cardinality();

    unsigned index() const {
        static_assert(N > 0); // 0 size orientations are a no go

        unsigned n;
        if constexpr (even) { // most puzzles have a 0 net orientation
            assert(N > 1); // trivial case
            n = N - 1;
        }
        else n = N;

        unsigned ret = 0;
        unsigned base = 1;
        for (unsigned k = 0; k < n; ++k) {
            assert((*this)[k] < v);
            ret *= v;
            ret += (*this)[k];
        }
        return ret;
    }

    void set_from_index(unsigned c) {
        static_assert(N > 0); // 0 size orientations are a no go

        unsigned n;
        if constexpr (even) {
            assert(N > 1); // trivial case
            n = N - 1;
            unsigned s = 0;
        }
        else n = N;
        
        unsigned s = 0; // unused for non even global orientation
        for (unsigned i = n - 1; i < N; --i){
            (*this)[i] = c % v;
            s += (*this)[i];
            c = c / v;
        }
        if constexpr (even) {
            (*this)[n] = (v - (s % v)) % v;
        }
    }

    bool is_solved() const {
        for (unsigned k : *this){
            if (k != 0) return false;
        }
        return true;
    }
};

template<unsigned N, unsigned M>
struct Center : std::array<unsigned, N> {
    // Holds the position of one color pieces, like the center pieces
    // of an NxNxN cubic puzzle
    // N = total number of pieces
    // M = number of colors (faces)
    // M must divide N
    // NP number of equivalent pieces on each face
    // For N=12 and M=4 the
    // solved state is {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3}

    static constexpr unsigned NP = N / M; // number of equivalent pieces on each face
    Center() {
        for (unsigned k = 0; k < N; ++k) {
            this->operator[](k) = k / NP;
        }
    }
    template<typename... Args>
    Center(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    template<unsigned K = 0>
    static constexpr unsigned cardinality() {
        if constexpr (K == M - 1) return 1;
        else {
            return binomial(N - K * NP, NP) * cardinality<K + 1>();
        }
    }
    static constexpr unsigned CARD = cardinality();

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k) {
            if (this->operator[](k) != k / NP) return false;
        }
        return true;
    }

    template<unsigned K = 0>
    unsigned index() const {
        if constexpr (K == M - 1) return 0;
        else {
            std::array<unsigned, N - NP * K> layout;
            unsigned k = 0;
            for (unsigned i = 0; i < N; ++i) {
                if ((*this)[i] == K) {layout[k] = 1; ++k;}
                else if ((*this)[i] > K) {layout[k] = 0; ++k;}
            }
            return layout_index(layout, NP) + binomial(N - NP * K, NP) * index<K + 1>();
        }
    }

    template<unsigned K = 0>
    void set_from_index(const unsigned c) {
        if constexpr(K == 0) this->fill(M); // fill with inconsistent value
        if constexpr(K == M - 1){
            for (unsigned i = 0; i < N; ++i){
                if ((*this)[i] >= M) {(*this)[i] = M - 1;}
            }
        } else {
            std::array<unsigned, N - K * NP> loc_layout;
            unsigned k = 0;
            layout_from_index(c % binomial(N - K * NP, NP), loc_layout, NP);
            for (unsigned i = 0; i < N; ++i) {
                if ((*this)[i] >= M) {
                    if (loc_layout[k] == 1) {
                        (*this)[i] = K;
                    }
                    ++k;
                }
            }
            assert(k == N - K * NP);
            set_from_index<K + 1>(c / binomial(N - K * NP, NP));
        }
    }
};

template<unsigned N>
void permute(std::array<unsigned, N> &items, const Permutation<N> &perm) {
    std::array<unsigned, N> buf = items;
    for (unsigned k = 0; k < N; ++k){
        items[k] = buf[perm[k]];
    }
}