#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>

#include "gtest/gtest.h"
#include "BaseGraph/fileio.h"


using namespace std;


class SmallGraphChar: public::testing::Test{
    public:
        BaseGraph::VertexLabeledUndirectedGraph<unsigned char> graph;
        void SetUp(){
            graph.addVertex(10);
            graph.addVertex(20);
            graph.addVertex(30);
            graph.addVertex(40);
            graph.addVertex(50);

            graph.addEdge(10, 30);
            graph.addEdge(10, 40);
            graph.addEdge(10, 50);
            graph.addEdge(20, 30);
            graph.addEdge(20, 40);
            graph.addEdge(20, 50);
            graph.addEdge(30, 40);
            graph.addEdge(40, 50);
        }
};

class SmallDirectedGraphChar: public::testing::Test{
    public:
        BaseGraph::VertexLabeledDirectedGraph<unsigned char> graph;
        void SetUp(){
            graph.addVertex(10);
            graph.addVertex(20);
            graph.addVertex(30);
            graph.addVertex(40);
            graph.addVertex(50);

            graph.addEdge(10, 30);
            graph.addEdge(10, 40);
            graph.addEdge(10, 50);
            graph.addEdge(20, 30);
            graph.addEdge(20, 40);
            graph.addEdge(20, 50);
            graph.addEdge(30, 40);
            graph.addEdge(40, 50);
        }
};

class SmallGraph2: public::testing::Test{
    public:
        BaseGraph::DirectedGraph graph = BaseGraph::DirectedGraph(10);

        void SetUp() {
            graph.resize(10);
            graph.addEdgeIdx( 0,  1, true);
            graph.addEdgeIdx( 0,  1, true);
            graph.addEdgeIdx( 0,  1, true);
            graph.addEdgeIdx( 1,  0, true);
            graph.addEdgeIdx( 1,  1, true);
            graph.addEdgeIdx( 1,  1, true);
            graph.addEdgeIdx( 1,  2, true);
            graph.addEdgeIdx( 1,  3, true);
            graph.addEdgeIdx( 1,  5, true);
            graph.addEdgeIdx( 4,  5, true);
            graph.addEdgeIdx( 5,  6, true);
            graph.addEdgeIdx( 6,  7, true);
            graph.addEdgeIdx( 7,  1, true);
            graph.addEdgeIdx( 7,  2, true);
            graph.addEdgeIdx( 7,  6, true);
            graph.addEdgeIdx( 7,  6, true);
            graph.addEdgeIdx( 7,  6, true);
            graph.addEdgeIdx( 9,  9, true);
        }
};

TEST(DirectedGraph, when_writingEdgeListIdxInTextFileAndReloadingIt_expect_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::loadDirectedEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(DirectedGraph, when_writingEdgeListIdxInBinaryFileAndReloadingIt_expect_allEdgesExist){
    BaseGraph::DirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "testGraph_tmp.bin");
    BaseGraph::DirectedGraph loadedGraph = BaseGraph::loadDirectedEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST(UndirectedGraph, when_writingEdgeListIdxInTextFileAndReloadingIt_expect_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::loadUndirectedEdgeListIdxFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.txt");
}

TEST(UndirectedGraph, when_writingEdgeListIdxInBinaryFileAndReloadingIt_expect_allEdgesExist){
    BaseGraph::UndirectedGraph graph(15);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(3, 14);

    BaseGraph::writeEdgeListIdxInBinaryFile(graph, "testGraph_tmp.bin");
    BaseGraph::UndirectedGraph loadedGraph = BaseGraph::loadUndirectedEdgeListIdxFromBinaryFile("testGraph_tmp.bin");

    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 1));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(0, 2));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(3, 14));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(1, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(2, 0));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(14, 3));

    remove("testGraph_tmp.bin");
}

TEST_F(SmallGraphChar, when_writingEdgeListInTextFileAndReloadingIt_expect_allEdgesAndVerticesExist){
    BaseGraph::writeEdgeListInTextFile(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::loadUndirectedEdgeListFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isVertex("10"));
    EXPECT_TRUE(loadedGraph.isVertex("20"));
    EXPECT_TRUE(loadedGraph.isVertex("30"));
    EXPECT_TRUE(loadedGraph.isVertex("40"));
    EXPECT_TRUE(loadedGraph.isVertex("50"));

    EXPECT_TRUE(loadedGraph.isEdge("10", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("30", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("40", "50"));

    remove("testGraph_tmp.txt");
}

TEST_F(SmallGraphChar, when_writingEdgeListInBinaryAndReloadIt_expect_graphContainsAllVerticesAndEdges){
    BaseGraph::writeEdgeListInBinaryFile(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::loadUndirectedEdgeListFromBinaryFile<unsigned char>("edgeList_tmp.bin");

    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));

    EXPECT_TRUE(loadedGraph.isEdge(10, 30));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(10, 50));
    EXPECT_TRUE(loadedGraph.isEdge(20, 30));
    EXPECT_TRUE(loadedGraph.isEdge(20, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 50));
    EXPECT_TRUE(loadedGraph.isEdge(30, 40));
    EXPECT_TRUE(loadedGraph.isEdge(40, 50));

    remove("edgeList_tmp.bin");
}

TEST_F(SmallDirectedGraphChar, when_writingEdgeListInTextFileAndReloadingIt_expect_allEdgesAndVerticesExist){
    BaseGraph::writeEdgeListInTextFile(graph, "testGraph_tmp.txt");
    auto loadedGraph = BaseGraph::loadDirectedEdgeListFromTextFile("testGraph_tmp.txt");

    EXPECT_TRUE(loadedGraph.isVertex("10"));
    EXPECT_TRUE(loadedGraph.isVertex("20"));
    EXPECT_TRUE(loadedGraph.isVertex("30"));
    EXPECT_TRUE(loadedGraph.isVertex("40"));
    EXPECT_TRUE(loadedGraph.isVertex("50"));

    EXPECT_TRUE(loadedGraph.isEdge("10", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("10", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "30"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("20", "50"));
    EXPECT_TRUE(loadedGraph.isEdge("30", "40"));
    EXPECT_TRUE(loadedGraph.isEdge("40", "50"));

    EXPECT_FALSE(loadedGraph.isEdge("30", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "10"));
    EXPECT_FALSE(loadedGraph.isEdge("30", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "20"));
    EXPECT_FALSE(loadedGraph.isEdge("40", "30"));
    EXPECT_FALSE(loadedGraph.isEdge("50", "40"));

    remove("testGraph_tmp.txt");
}


TEST_F(SmallDirectedGraphChar, when_writingEdgeListInBinaryAndReloadIt_expect_graphContainsAllVerticesAndEdges){
    BaseGraph::writeEdgeListInBinaryFile(graph, "edgeList_tmp.bin");
    auto loadedGraph = BaseGraph::loadDirectedEdgeListFromBinaryFile<unsigned char>("edgeList_tmp.bin");

    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));

    EXPECT_TRUE(loadedGraph.isEdge(10, 30));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(10, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 30));
    EXPECT_TRUE(loadedGraph.isEdge(20, 40));
    EXPECT_TRUE(loadedGraph.isEdge(20, 50));
    EXPECT_TRUE(loadedGraph.isEdge(30, 40));
    EXPECT_TRUE(loadedGraph.isEdge(40, 50));

    EXPECT_FALSE(loadedGraph.isEdge(30, 10));
    EXPECT_FALSE(loadedGraph.isEdge(40, 10));
    EXPECT_FALSE(loadedGraph.isEdge(40, 10));
    EXPECT_FALSE(loadedGraph.isEdge(30, 20));
    EXPECT_FALSE(loadedGraph.isEdge(40, 20));
    EXPECT_FALSE(loadedGraph.isEdge(50, 20));
    EXPECT_FALSE(loadedGraph.isEdge(40, 30));
    EXPECT_FALSE(loadedGraph.isEdge(50, 40));

    remove("edgeList_tmp.bin");
}

TEST(loadFromEdgeListBinary, when_loadingNonExistingEdgeListBinary_expect_throwRuntimeError){
    EXPECT_THROW(BaseGraph::loadUndirectedEdgeListFromBinaryFile<bool>("\0"), runtime_error);
}

TEST(loadFromTextFile, when_loadingNonExistingEdgeListTextFile_expect_throwRuntimeError){
    EXPECT_THROW(BaseGraph::loadUndirectedEdgeListFromTextFile("\0"), runtime_error);
}

TEST_F(SmallGraphChar, when_writingVerticesBinaryAndReloadThem_except_graphContainsCorrectVertices){
    writeVerticesInBinaryFile(graph, "verticesList_tmp.bin");
    BaseGraph::VertexLabeledUndirectedGraph<unsigned char> loadedGraph;
    addVerticesFromBinaryFile(loadedGraph, "verticesList_tmp.bin");

    EXPECT_TRUE(loadedGraph.isVertex(10));
    EXPECT_TRUE(loadedGraph.isVertex(20));
    EXPECT_TRUE(loadedGraph.isVertex(30));
    EXPECT_TRUE(loadedGraph.isVertex(40));
    EXPECT_TRUE(loadedGraph.isVertex(50));
    remove("verticesList_tmp.bin");
}


TEST(addVerticesFromBinaryFile, when_loadingNonExistingVerticesBinaryFile_expect_throwRuntimeError){
    BaseGraph::VertexLabeledUndirectedGraph<bool> graph;
    EXPECT_THROW(addVerticesFromBinaryFile(graph, "\0"), runtime_error);
}


TEST_F(SmallGraph2, when_loadingGraphFromEdgelist_expect_graphContainsAllVerticesAndEdges){
    writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");

    BaseGraph::DirectedGraph loadedGraph(1);
    bool allow_multiedges = true;
    bool allow_selfloops = true;
    auto Name2Num = BaseGraph::loadGraphFromEdgelist("testGraph_tmp.txt", loadedGraph, allow_multiedges, allow_selfloops);

    EXPECT_EQ(Name2Num.size(), 9);
    EXPECT_EQ(loadedGraph.getSize(), 9);
    EXPECT_EQ(loadedGraph.getEdgeNumber(), 18);
    EXPECT_TRUE(loadedGraph.isEdgeIdx(Name2Num["1"], Name2Num["1"]));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(Name2Num["9"], Name2Num["9"]));
    EXPECT_EQ(loadedGraph.getOutDegrees(), vector<size_t>({3, 6, 0, 0, 1, 1, 1, 5, 1}));
    EXPECT_EQ(loadedGraph.getInDegrees(), vector<size_t>({1, 6, 2, 1, 2, 0, 4, 1, 1}));

    remove("testGraph_tmp.txt");
}


TEST_F(SmallGraph2, when_loadingGraphFromEdgelist_expect_graphIgnoresSelfloops){
    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");

    BaseGraph::DirectedGraph loadedGraph(1);
    bool allow_multiedges = true;
    bool allow_selfloops = false;
    auto Name2Num = BaseGraph::loadGraphFromEdgelist("testGraph_tmp.txt", loadedGraph, allow_multiedges, allow_selfloops);

    EXPECT_EQ(Name2Num.size(), 8);
    EXPECT_EQ(loadedGraph.getSize(), 8);
    EXPECT_EQ(loadedGraph.getEdgeNumber(), 15);
    EXPECT_FALSE(loadedGraph.isEdgeIdx(Name2Num["1"], Name2Num["1"]));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(Name2Num["9"], Name2Num["9"]));
    EXPECT_EQ(loadedGraph.getOutDegrees(), vector<size_t>({3, 4, 0, 0, 1, 1, 1, 5}));
    EXPECT_EQ(loadedGraph.getInDegrees(), vector<size_t>({1, 4, 2, 1, 2, 0, 4, 1}));

    remove("testGraph_tmp.txt");
}


TEST_F(SmallGraph2, when_loadingGraphFromEdgelist_expect_graphIgnoresMultiedges){
    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");

    BaseGraph::DirectedGraph loadedGraph(1);
    bool allow_multiedges = false;
    bool allow_selfloops = true;
    auto Name2Num = BaseGraph::loadGraphFromEdgelist("testGraph_tmp.txt", loadedGraph, allow_multiedges, allow_selfloops);

    EXPECT_EQ(Name2Num.size(), 9);
    EXPECT_EQ(loadedGraph.getSize(), 9);
    EXPECT_EQ(loadedGraph.getEdgeNumber(), 13);
    EXPECT_TRUE(loadedGraph.isEdgeIdx(Name2Num["1"], Name2Num["1"]));
    EXPECT_TRUE(loadedGraph.isEdgeIdx(Name2Num["9"], Name2Num["9"]));
    EXPECT_EQ(loadedGraph.getOutDegrees(), vector<size_t>({1, 5, 0, 0, 1, 1, 1, 3, 1}));
    EXPECT_EQ(loadedGraph.getInDegrees(), vector<size_t>({1, 3, 2, 1, 2, 0, 2, 1, 1}));

    remove("testGraph_tmp.txt");
}


TEST_F(SmallGraph2, when_loadingGraphFromEdgelist_expect_graphIgnoresMultiedgesAndSelfloops){
    BaseGraph::writeEdgeListIdxInTextFile(graph, "testGraph_tmp.txt");

    BaseGraph::DirectedGraph loadedGraph(1);
    bool allow_multiedges = false;
    bool allow_selfloops = false;
    auto Name2Num = BaseGraph::loadGraphFromEdgelist("testGraph_tmp.txt", loadedGraph, allow_multiedges, allow_selfloops);

    EXPECT_EQ(Name2Num.size(), 8);
    EXPECT_EQ(loadedGraph.getSize(), 8);
    EXPECT_EQ(loadedGraph.getEdgeNumber(), 11);
    EXPECT_FALSE(loadedGraph.isEdgeIdx(Name2Num["1"], Name2Num["1"]));
    EXPECT_FALSE(loadedGraph.isEdgeIdx(Name2Num["9"], Name2Num["9"]));
    EXPECT_EQ(loadedGraph.getOutDegrees(), vector<size_t>({1, 4, 0, 0, 1, 1, 1, 3}));
    EXPECT_EQ(loadedGraph.getInDegrees(), vector<size_t>({1, 2, 2, 1, 2, 0, 2, 1}));

    remove("testGraph_tmp.txt");
}
