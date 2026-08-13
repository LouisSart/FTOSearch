#include <array>
#include <cassert>

template<unsigned N>
struct Permutation : std::array<unsigned, N> {
    Permutation() { // Constructeur sans arguments
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }
    template<typename... Args>
    Permutation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k){
            if (this->operator[](k) != k) return false;
        }
        return true;
    }
    void reset() {
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = k;
        }
    }
    void compose(const Permutation<N> & other) {
        Permutation<N> ret = *this;
        for (unsigned k = 0; k < N; ++k){
            this->operator[](k) = ret[other[k]];
        }
    }
    unsigned index(const bool even = false) const {
        // Compute the lexicographic index of the permutation
        static_assert(N > 0); // empty permutations are a problem
        // If the permutation necessarily has even parity,
        // do not encode the information of the last two digits
        unsigned n;
        if (even) {n = N - 1;
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
    void set_from_index(unsigned c, const bool even = false){
        // Reconstruct the permutation having index c
        static_assert(N > 0); // empty permutations are a problem

        if (even) {
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
};

template<unsigned N>
struct Orientation : std::array<unsigned, N> {
    Orientation() { // Constructeur sans arguments
        std::array<unsigned, N>::fill(0);
    }
    template<typename... Args>
    Orientation(Args... args) : std::array<unsigned, N>{{ static_cast<unsigned>(args)... }} {} // Constructeur par brace-enclosed

    bool is_solved() const {
        for (unsigned k : *this){
            if (k != 0) return false;
        }
        return true;
    }
};

template<unsigned N, unsigned M>
struct Triangles : std::array<unsigned, N> {
    // For N=12 and M=3 the
    // solved_state is {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3}
    Triangles() {
        for (unsigned k = 0; k < N; ++k) {
            this->operator[](k) = k / M;
        }
    }

    bool is_solved() const {
        for (unsigned k = 0; k < N; ++k) {
            if (this->operator[](k) != k / M) return false;
        }
        return true;
    }
};

template<unsigned N>
void permute(std::array<unsigned, N> &items, const Permutation<N> &perm) {
    std::array<unsigned, N> buf = items;
    for (unsigned k = 0; k < N; ++k){
        items[k] = buf[perm[k]];
    }
}