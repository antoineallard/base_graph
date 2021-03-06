#ifndef BASE_GRAPH_TYPES
#define BASE_GRAPH_TYPES

#include <cstddef>
#include <vector>
#include <list>


namespace BaseGraph{

typedef size_t VertexIndex;
typedef std::pair<VertexIndex, VertexIndex> Edge;
typedef std::list<VertexIndex> Successors;
typedef std::vector<Successors> AdjacencyLists;
typedef std::vector<std::vector<size_t>> AdjacencyMatrix;

typedef unsigned int EdgeMultiplicity;

} // namespace BaseGraph

#endif
