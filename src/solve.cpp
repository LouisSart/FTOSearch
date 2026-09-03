#include <algorithm>
#include "solve.hpp"
#include "coordinate_fto.hpp"
#include "../lib/pruning_table.hpp"

fs::path table_dir = "pruning_tables";
fs::path corner_table_path = table_dir / "corners";
fs::path edge_table_path = table_dir / "edges";
fs::path triangle_table_path = table_dir / "triangles";
fs::path edge_convert_table_path = table_dir / "edge_convert";

PruningTable<CORNER_CARD> corner_table;
PruningTable<EDGE_CARD> edge_table;
PruningTable<TRIANGLE_CARD> triangle_table;
std::array<unsigned, EDGE_CARD * 2> edge_conversion;

void generate_edge_convert_table() {
    // Build a conversion table to retrieve the global 
    // permutation index from the split indices e1 and e2.
    Permutation<12, true> edges;
    edge_conversion.fill(EDGE_CARD);
    for (unsigned c = 0; c < EDGE_CARD; ++c){
        edges.set_from_index(c);
        auto [cl, c1, c2] = edges.split_indices();

        unsigned e1 = cl * SIX_EDGE_PERM_CARD + c1;
        unsigned sparse_idx = e1 * SIX_EDGE_PERM_CARD + c2;
        assert(sparse_idx < EDGE_CARD * 2);
        edge_conversion[sparse_idx] = c;
    }
}

void write_edge_convert_table() {
    fs::create_directories(edge_convert_table_path.parent_path());
    std::ofstream file(edge_convert_table_path, std::ios::binary);
    file.write(reinterpret_cast<const char *>(edge_conversion.data()),
                sizeof(unsigned) * EDGE_CARD * 2);
    file.close();
}

bool load_edge_convert_table() {
    if (fs::exists(edge_convert_table_path)) {
        std::ifstream istrm(edge_convert_table_path, std::ios::binary);
        istrm.read(reinterpret_cast<char *>(edge_conversion.data()),
                    sizeof(unsigned) * EDGE_CARD * 2);
        istrm.close();
        return true;
    } else {
        print("Edge convert table not found at: ", edge_convert_table_path);
    }
    return false;
}

unsigned dense_edge_index(const FTO& fto){
    return edge_conversion[edge_index(fto)];
}

void edges_from_dense_index(const unsigned &c, FTO& fto) {
    static CubieFTO cfto;
    cfto.ep.set_from_index(c);
    fto.e1 = e1_index(cfto);
    fto.e2 = e2_index(cfto);
}

void generate_corner_table(){  
    print("Generating corner pruning table");  
    corner_table.generate<FTO, true>(corner_index, corners_from_index, moves, 3, 4);
    corner_table.write(corner_table_path);
    // corner_table.show_distribution();
}

void generate_edge_table(){  
    print("Generating edge index conversion table");
    generate_edge_convert_table();
    write_edge_convert_table();
    print("Generating edge pruning table");
    edge_table.generate<FTO, true>(dense_edge_index, edges_from_dense_index, moves, 5, 11);
    edge_table.write(edge_table_path);
    // edge_table.show_distribution();
}

void generate_triangle_table(){
    print("Generating triangle pruning table");
    triangle_table.generate<FTO, true>(tri1_index, tri1_from_index, moves, 3, 7);
    triangle_table.write(triangle_table_path);
    // triangle_table.show_distribution();
}

void generate_pruning_tables() {
    generate_corner_table();
    generate_edge_table();
    generate_triangle_table();
};

bool load_pruning_tables() {
    if (corner_table.load(corner_table_path)
        && edge_table.load(edge_table_path)
        && load_edge_convert_table()
        && triangle_table.load(triangle_table_path)) return true;
    print("Pruning tables missing, generate first");
    return false;
}

unsigned estimate(const CubieFTO &fto){
    return std::max({corner_table.estimate(fto.corner_index()),
                    edge_table.estimate(fto.ep.index()),
                    triangle_table.estimate(fto.tri1.index()),
                    triangle_table.estimate(fto.tri2.index())}
        );
};

unsigned estimate(const FTO& fto) {
    return std::max({
        corner_table.estimate(corner_index(fto)),
        edge_table.estimate(edge_conversion[edge_index(fto)]),
        triangle_table.estimate(tri1_index(fto)),
        triangle_table.estimate(tri2_index(fto))
    });
}

Solutions<CubieFTO> optimal(const CubieFTO &fto, const unsigned max_depth){
    
    auto root = make_root(fto);
    return IDAstar<true, CubieFTO>(root, estimate, is_solved, standard_directions<CubieFTO>, max_depth);
}

Solutions<FTO> optimal(const FTO &fto, const unsigned max_depth){
    // if (!load_move_tables()) generate_move_tables(); // CHECKME : pourquoi on ne peut pas loader ici ?

    auto root = make_root(fto);
    return IDAstar<true, FTO>(root, estimate, is_solved, standard_directions<FTO>, max_depth);
}