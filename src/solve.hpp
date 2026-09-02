#include "../lib/search.hpp"
#include "fto.hpp"

using NodePtr = Node<CubieFTO>::sptr;
std::vector<Move> standard_directions(const NodePtr node);
Solutions<NodePtr> optimal(const CubieFTO &fto);


void generate_corner_table();
void generate_edge_table();
void generate_triangle_table();
void load_corner_table();
void load_edge_table();
void load_triangle_table();

void generate_pruning_tables();
bool load_pruning_tables();

unsigned estimate(const CubieFTO& fto);