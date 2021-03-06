#ifndef BASE_GRAPH_VERTEX_LABELED_GRAPH_HPP
#define BASE_GRAPH_VERTEX_LABELED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace BaseGraph{

template<typename GraphBase, typename VertexLabel, bool isHashable>
class VertexLabeledGraph: public GraphBase {
    private:
        struct Void {};
        template<typename U> struct __HashTable { std::unordered_map<U, VertexIndex> table; };

        typedef typename std::conditional<isHashable, __HashTable<VertexLabel>, Void>::type HashTable;
        HashTable verticesMapping;

    protected:
        std::vector<VertexLabel> vertices;

    public:
        VertexLabeledGraph(): GraphBase() {}
        VertexLabeledGraph(const std::list<std::pair<VertexLabel, VertexLabel>>& edgeList);
        VertexLabeledGraph(const GraphBase& source, const std::vector<VertexLabel>& vertices);
        const std::vector<VertexLabel>& getVertices() const { return vertices; }

        template<bool otherHashable>
            bool operator==(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const;
        template<bool otherHashable>
            bool operator!=(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const { return !(this->operator==(other)); };


        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            addVertex(const VertexLabel& vertex, bool force=false);
        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            addVertex(const VertexLabel& vertex, bool force=false);
        bool isVertex(const VertexLabel& vertex) const;
        const VertexLabel& getLabelFromIndex(VertexIndex vertexIdx) const { this->assertVertexInRange(vertexIdx); return vertices[vertexIdx]; }
        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<_hashable, const VertexIndex&>::type
            findVertexIndex(const VertexLabel& vertex) const;
        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<!_hashable, const VertexIndex>::type
            findVertexIndex(const VertexLabel& vertex) const;
        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<_hashable>::type
            changeVertexLabelTo(const VertexLabel& currentLabel, const VertexLabel& newLabel);
        template<typename... Dummy, bool _hashable=isHashable> typename std::enable_if<!_hashable>::type
            changeVertexLabelTo(const VertexLabel& currentLabel, const VertexLabel& newLabel);
        void removeVertexFromEdgeList(VertexLabel vertex) { this->removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };

        void addEdge(VertexLabel source, VertexLabel destination, bool force=false) { this->addEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); }
        bool isEdge(VertexLabel source, VertexLabel destination) const { return this->isEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        void removeEdge(VertexLabel source, VertexLabel destination) { this->removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };


        size_t getInDegree(VertexLabel vertex) const { return this->getInDegreeIdx(findVertexIndex(vertex)); }
        size_t getOutDegree(VertexLabel vertex) const { return this->getOutDegreeIdx(findVertexIndex(vertex)); }


        template<typename Graph>
        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledGraph<GraphBase, VertexLabel, isHashable>& graph) {
            stream << "Vertex labeled graph of" << typeid(Graph).name() << "graph of size: " << graph.getSize() << "\n"
                   << "Vertex label type \"" << typeid(VertexLabel).name() << "\" used as " << (isHashable ? "hashable": "not hashable") << "\n"
                   << "Neighbours of:\n";

            for (VertexIndex i: graph) {
                stream << graph.vertices[i] << ": ";
                auto& outEdges = graph.getOutEdgesOfIdx(i);
                for (auto& neighbour: outEdges)
                    stream << (neighbour==*outEdges.begin() ? "" : ", ")
                           << graph.vertices[neighbour];
                stream << "\n";
            }
            return stream;
        }
};

template <typename VertexLabel, bool isHashable=false>
using VertexLabeledDirectedGraph = VertexLabeledGraph<DirectedGraph, VertexLabel, isHashable>;

template <typename VertexLabel, bool isHashable=false>
using VertexLabeledUndirectedGraph = VertexLabeledGraph<UndirectedGraph, VertexLabel, isHashable>;

template <typename VertexLabel, typename EdgeLabel, bool isHashable=false>
using VertexAndEdgeLabeledDirectedGraph = VertexLabeledGraph<EdgeLabeledDirectedGraph<EdgeLabel>, VertexLabel, isHashable>;

template <typename VertexLabel, typename EdgeLabel, bool isHashable=false>
using VertexAndEdgeLabeledUndirectedGraph = VertexLabeledGraph<EdgeLabeledUndirectedGraph<EdgeLabel>, VertexLabel, isHashable>;



template<typename GraphBase, typename VertexLabel, bool isHashable>
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::VertexLabeledGraph(const std::list<std::pair<VertexLabel, VertexLabel>>& edgeList) {
    for (auto& edge: edgeList) {
        // By default addVertex does not add existing labels
        addVertex(edge.first);
        addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::VertexLabeledGraph(const GraphBase& source, const std::vector<VertexLabel>& verticesNames) {
    if (source.getSize() != verticesNames.size())
        throw std::invalid_argument("The vertices vector must be the size of the graph");

    this->edgeNumber = 0;
    for (auto vertex: verticesNames) {
        if (isVertex(vertex))
            throw std::invalid_argument("Couldn't create vertex labeled graph from directed graph: duplicate in vertices names");
        else
            addVertex(vertex, true);
    }

    for (VertexIndex& vertex: source)
        for (const VertexIndex& neighbour: source.getOutEdgesOfIdx(vertex))
            this->addEdgeIdx(vertex, neighbour);
}


template<typename GraphBase, typename VertexLabel, bool isHashable>
template<bool otherHashable> 
bool VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::operator==(const VertexLabeledGraph<GraphBase, VertexLabel, otherHashable>& other) const{
    bool sameObject = this->size == other.size;
    auto& _adjacencyList = this->adjacencyList;

    std::list<VertexIndex>::const_iterator it;
    try {
    for (VertexIndex i=0; i<this->size && sameObject; ++i){
        if (!other.isVertex(vertices[i])) sameObject = false;
        if (!isVertex(other.vertices[i])) sameObject = false;

        for (it=_adjacencyList[i].begin(); it != _adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdge(vertices[i], vertices[*it]))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdge(other.vertices[i], other.vertices[*it]))
                sameObject = false;
        }
    }
    } catch (std::invalid_argument&) {  // isEdge calling findVertexIndex threw "Vertex does not exist"
        sameObject = false;
    }

    return sameObject;
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::addVertex(const VertexLabel& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        verticesMapping.table[vertex] = vertices.size();
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::addVertex(const VertexLabel& vertex, bool force) {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call addVertex");

    if (force || !isVertex(vertex)) {
        vertices.push_back(vertex);
        this->adjacencyList.push_back(std::list<VertexIndex>());
        this->size++;
    }
}


template<typename GraphBase, typename VertexLabel, bool isHashable>
bool VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::isVertex(const VertexLabel& vertex) const{
    bool exists = false;

    for (VertexIndex i=0; i<this->size && !exists; ++i)
        if (vertices[i] == vertex)
            exists = true;
    return exists;
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<_hashable, const VertexIndex&>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::findVertexIndex(const VertexLabel& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");

    if (verticesMapping.table.find(vertex) != verticesMapping.table.end())
        return verticesMapping.table.at(vertex);
    throw std::invalid_argument("Vertex does not exist");
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<!_hashable, const VertexIndex>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::findVertexIndex(const VertexLabel& vertex) const {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call findVertexIndex");
    for (VertexIndex& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::invalid_argument("Vertex does not exist");
}


template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<_hashable>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::changeVertexLabelTo(const VertexLabel& currentLabel, const VertexLabel& newLabel){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newLabel)) throw std::invalid_argument("The object is already used as an attribute by another vertex.");

    VertexIndex vertexIndex = findVertexIndex(currentLabel);
    vertices[vertexIndex] = newLabel;
    verticesMapping.table.erase(currentLabel);
    verticesMapping.table[newLabel] = vertexIndex;
}

template<typename GraphBase, typename VertexLabel, bool isHashable>
template<typename... Dummy, bool _hashable>
typename std::enable_if<!_hashable>::type
VertexLabeledGraph<GraphBase, VertexLabel, isHashable>::changeVertexLabelTo(const VertexLabel& currentLabel, const VertexLabel& newLabel){
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments to call changeVertexObjectTo");
    if (isVertex(newLabel)) throw std::invalid_argument("newLabel is already used as an attribute by another vertex.");

    vertices[findVertexIndex(currentLabel)] = newLabel;
}

} // namespace BaseGraph

#endif
