#include "coordinate_fto.hpp"
#include "../lib/search.hpp"

namespace fs = std::filesystem;

namespace RLBD {
    void generate_pruning_table();
    Solutions<FTO> optimal(const FTO &, const unsigned m = 23);
};

namespace reduction {
Solutions<FTO> solve_centers(const FTO&, const unsigned m = 23);
void edge_coset();
void generate_e2_ptable();
};