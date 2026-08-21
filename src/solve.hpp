#include "search.hpp"
#include "fto.hpp"

using NodePtr = Node<CubieFTO>::sptr;
std::vector<Move> standard_directions(const NodePtr node);
Solutions<NodePtr> optimal(const CubieFTO &fto);