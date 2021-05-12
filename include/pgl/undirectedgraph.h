#ifndef PGL_UNDIRECTED_GRAPH_BASE_H
#define PGL_UNDIRECTED_GRAPH_BASE_H

#include "pgl/directedgraph.h"


namespace PGL{

class UndirectedGraph: public DirectedGraph{
    public:
        explicit UndirectedGraph(size_t graphSize=10): DirectedGraph(graphSize) {};

        bool operator==(const UndirectedGraph& other) const;
        bool operator!=(const UndirectedGraph& other) const { return !(this->operator==(other)); };

        void addEdgeIdx(size_t source, size_t destination, bool force=false);
        void removeEdgeIdx(size_t source, size_t destination);
        void removeVertexFromEdgeListIdx(size_t vertex);
        void removeMultiedges();

        const std::list<size_t>& getNeighboursOfIdx(size_t vertex) const { return getOutEdgesOfIdx(vertex); };
        size_t getDegreeIdx(size_t vertex) const { return DirectedGraph::getOutDegreeIdx(vertex); };
        std::vector<size_t> getDegrees() const;
};

} // namespace PGL

#endif
