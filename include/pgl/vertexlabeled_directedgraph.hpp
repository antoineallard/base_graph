#ifndef PGL_VERTEX_LABELED_DIRECTED_GRAPH_HPP
#define PGL_VERTEX_LABELED_DIRECTED_GRAPH_HPP

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "pgl/directedgraph.h"
#include "pgl/algorithms/graphpaths.h"


namespace PGL{

template<typename T>
class VertexLabeledDirectedGraph: public DirectedGraph{

    protected:
        std::vector<T> vertices;

    public:
        VertexLabeledDirectedGraph(): DirectedGraph(0) {};
        VertexLabeledDirectedGraph(const std::list<std::pair<T, T>>& edgeList);
        VertexLabeledDirectedGraph(const VertexLabeledDirectedGraph<T>& source);
        VertexLabeledDirectedGraph(const DirectedGraph& source, const std::vector<T>& vertices);

        VertexLabeledDirectedGraph<T> operator=(const VertexLabeledDirectedGraph<T>& other);
        bool operator==(const VertexLabeledDirectedGraph<T>& other) const;
        bool operator!=(const VertexLabeledDirectedGraph<T>& other) const { return !(this->operator==(other)); };


        void addVertex(T vertex, bool force=false);
        bool isVertex(T vertex) const;
        void changeVertexObjectTo(T currentObject, T newObject);
        void removeVertexFromEdgeList(T vertex) { removeVertexFromEdgeListIdx(findVertexIndex(vertex)); };
        const T& getVertexFromIdx(size_t vertexIdx) const;
        size_t findVertexIndex(T vertex) const;

        const size_t getSize() const {return size;}
        const std::vector<T>& getVertices() const { return vertices; };
        const std::list<T> getOutEdgesOf(T vertex) const;

        std::list<T> convertIndicesListToObjects(const std::list<size_t>& indicesList) const;
        std::vector<T> convertIndicesVectorToObjects(const std::vector<size_t>& indicesVector) const;

        void addEdge(T source, T destination, bool force=false) { addEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); }
        void removeEdge(T source, T destination) { removeEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };
        bool isEdge(T source, T destination) const { return isEdgeIdx(findVertexIndex(source), findVertexIndex(destination)); };

        size_t getDegree(T vertex) const { return getDegreeIdx(findVertexIndex(vertex)); };


        friend std::ostream& operator <<(std::ostream &stream, const VertexLabeledDirectedGraph<T>& graph) {
                for (size_t i=0; i<graph.size; ++i){
                    stream << "Vertex " << graph.vertices[i] << ": ";
                    for (const size_t& neighbour: graph.getOutEdgesOfIdx(i))
                        stream << graph.vertices[neighbour] << ", ";
                    stream << "\n";
                }
                return stream;
            };
};

template<typename T>
VertexLabeledDirectedGraph<T>::VertexLabeledDirectedGraph(const VertexLabeledDirectedGraph<T>& source){
    size = source.size;
    vertices = source.vertices;
    adjacencyList = source.adjacencyList;
    edgeNumber = source.edgeNumber;
}

template<typename T>
VertexLabeledDirectedGraph<T>::VertexLabeledDirectedGraph(const std::list<std::pair<T, T>>& edgeList):
        DirectedGraph(0) {
    for (auto& edge: edgeList) {
        // By default addVertex does not add existing labels
        addVertex(edge.first);
        addVertex(edge.second);
        addEdge(edge.first, edge.second);
    }
}

template<typename T>
VertexLabeledDirectedGraph<T>::VertexLabeledDirectedGraph(const DirectedGraph& source, const std::vector<T>& verticesNames):
        DirectedGraph(source.getSize()) {
    if (source.getSize() != verticesNames.size())
        throw std::logic_error("The vertices vector must be the size of the graph");

    vertices = verticesNames;
    edgeNumber = 0;

    for (size_t& vertex: source)
        for (size_t& neighbour: source.getOutEdgesOfIdx(vertex))
            addEdgeIdx(vertex, neighbour);
}


template<typename T>
VertexLabeledDirectedGraph<T> VertexLabeledDirectedGraph<T>::operator=(const VertexLabeledDirectedGraph<T>& other){
    if (this != &other){
        size = other.size;
        vertices = other.vertices;
        adjacencyList = other.adjacencyList;
        edgeNumber = other.edgeNumber;
    }
    return *this;
}

template<typename T>
bool VertexLabeledDirectedGraph<T>::operator==(const VertexLabeledDirectedGraph<T>& other) const{
    bool sameObject = size == other.size;

    std::list<size_t>::const_iterator it;
    try {
    for (size_t i=0; i<size && sameObject; ++i){
        if (!other.isVertex(vertices[i])) sameObject = false;
        if (!isVertex(other.vertices[i])) sameObject = false;

        for (it=adjacencyList[i].begin(); it != adjacencyList[i].end() && sameObject; ++it){
            if (!other.isEdge(vertices[i], vertices[*it]))
                sameObject = false;
        }

        for (it=other.adjacencyList[i].begin(); it != other.adjacencyList[i].end() && sameObject; ++it){
            if (!isEdge(other.vertices[i], other.vertices[*it]))
                sameObject = false;
        }
    }
    } catch (std::logic_error) {  // isEdge calling findVertexIndex threw "Vertex does not exist"
        sameObject = false;
    }

    return sameObject;
}

template <typename T>
void VertexLabeledDirectedGraph<T>::addVertex(T vertex, bool force){
    if (force) {
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<size_t>());
        size++;
    }
    else if (!isVertex(vertex)) {
        vertices.push_back(vertex);
        adjacencyList.push_back(std::list<size_t>());
        size++;
    }
}

template <typename T>
bool VertexLabeledDirectedGraph<T>::isVertex(T vertex) const{
    bool exists = false;
    for (size_t i=0; i<size && !exists; ++i)
        if (vertices[i] == vertex) exists = true;
    return exists;
}

template<typename T>
const std::list<T> VertexLabeledDirectedGraph<T>::getOutEdgesOf(T vertex) const{
    return convertIndicesListToObjects(getOutEdgesOfIdx(findVertexIndex(vertex)));
}

template<typename T>
std::list<T> VertexLabeledDirectedGraph<T>::convertIndicesListToObjects(const std::list<size_t>& indicesList) const{
    std::list<T> returnedList;

    for (auto& element: indicesList) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedList.push_back(vertices[element]);
    }
    return returnedList;
}

template<typename T>
std::vector<T> VertexLabeledDirectedGraph<T>::convertIndicesVectorToObjects(const std::vector<size_t>& indicesVector) const{
    std::vector<T> returnedVector(indicesVector.size());

    for (auto& element: indicesVector) {
        if (element >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
        returnedVector[element] = vertices[element];
    }
    return returnedVector;
}

template<typename T>
void VertexLabeledDirectedGraph<T>::changeVertexObjectTo(T currentObject, T newObject){
    if (isVertex(newObject)) throw std::logic_error("The object is already used as an attribute by another vertex.");
    vertices[findVertexIndex(currentObject)] = newObject;
}

template<typename T>
size_t VertexLabeledDirectedGraph<T>::findVertexIndex(T vertex) const{
    for (size_t& i: *this)
        if (vertices[i] == vertex)
            return i;
    throw std::logic_error("Vertex does not exist");
}

template<typename T>
const T& VertexLabeledDirectedGraph<T>::getVertexFromIdx(size_t vertexIdx) const {
    if (vertexIdx >= size) throw std::out_of_range("The given list is invalid: index greater than the vertices size.");
    return vertices[vertexIdx];
}

} // namespace PGL

#endif
