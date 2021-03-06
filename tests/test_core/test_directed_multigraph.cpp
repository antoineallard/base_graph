#include <stdexcept>
#include <list>

#include "gtest/gtest.h"
#include "BaseGraph/directed_multigraph.h"


using LabeledSuccessors = BaseGraph::DirectedMultigraph::LabeledSuccessors;


TEST(DirectedMultigraph, addMultiedgeIdx_inexistent_newMultiedge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addMultiedgeIdx(0, 1, 3);
    graph.addMultiedgeIdx(0, 2, 1);
    graph.addEdgeIdx     (0, 0);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 3}, {2, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    5);
}

TEST(DirectedMultigraph, addMultiedgeIdx_existent_multiplicityIncremented) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 2);
    graph.addEdgeIdx     (0, 0);
    graph.addMultiedgeIdx(0, 2, 1);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {2, 2}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(DirectedMultigraph, addMultiedgeIdx_existentMultiedgeAndForce_newMultiedge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 2);
    graph.addEdgeIdx     (0, 0);
    graph.addMultiedgeIdx(0, 2, 1, true);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {2, 1}, {0, 1}, {2, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 4);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(DirectedMultigraph, addMultiedgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.addMultiedgeIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.addMultiedgeIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.addMultiedgeIdx(0, 1, 1), std::out_of_range);
}


TEST(DirectedMultigraph, removeMultiedgeIdx_existentEdgeWithHigherMultiplicity_multiplicityDecremented) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 2);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {2, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);
}

TEST(DirectedMultigraph, removeMultiedgeIdx_existentEdgeWithEqualMultiplicity_noEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 3);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(DirectedMultigraph, removeMultiedgeIdx_existentEdgeWithLowerMultiplicity_noEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addMultiedgeIdx(0, 2, 3);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(DirectedMultigraph, removeMultiedgeIdx_inexistentEdge_graphUnchanged) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx     (0, 1);
    graph.addEdgeIdx     (0, 0);

    graph.removeMultiedgeIdx(0, 2, 4);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{1, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(DirectedMultigraph, removeMultiedgeIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.removeMultiedgeIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.removeMultiedgeIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.removeMultiedgeIdx(0, 1, 1), std::out_of_range);
}


TEST(DirectedMultigraph, setEdgeMultiplicityIdx_inexistentEdgeToPositiveMultiplicity_addEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2);
    graph.setEdgeMultiplicityIdx(0, 1, 2);
    graph.addEdgeIdx            (0, 0);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {1, 2}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);
}

TEST(DirectedMultigraph, setEdgeMultiplicityIdx_inexistentEdgeToMultiplicity0_doNothing) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2);
    graph.setEdgeMultiplicityIdx(0, 1, 0);
    graph.addEdgeIdx            (0, 0);


    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(DirectedMultigraph, setEdgeMultiplicityIdx_existentEdgeToMultiplicity0_removeEdge) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx            (0, 2, 1);
    graph.addEdgeIdx            (0, 1, 1);
    graph.addEdgeIdx            (0, 0);
    graph.setEdgeMultiplicityIdx(0, 1, 0);

    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 2);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    2);
}

TEST(DirectedMultigraph, setEdgeMultiplicityIdx_existentEdgeToNonZeroMultiplicity_multiplicityAndEdgeNumberUpdated) {
    BaseGraph::DirectedMultigraph graph(3);
    graph.addEdgeIdx(0, 2);
    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(0, 0);

    graph.setEdgeMultiplicityIdx(0, 1, 1);
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {1, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);

    graph.setEdgeMultiplicityIdx(0, 1, 2);
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {1, 2}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    4);

    graph.setEdgeMultiplicityIdx(0, 1, 1);
    EXPECT_EQ(graph.getOutEdgesOfIdx(0), LabeledSuccessors({{2, 1}, {1, 1}, {0, 1}}));
    EXPECT_EQ(graph.getDistinctEdgeNumber(), 3);
    EXPECT_EQ(graph.getTotalEdgeNumber(),    3);
}

TEST(DirectedMultigraph, setEdgeMultiplicityIdx_vertexOutOfRange_throwOutOfRange) {
    BaseGraph::DirectedMultigraph graph(0);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(0, 0, 1), std::out_of_range);
    graph.resize(1);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(1, 0, 1), std::out_of_range);
    EXPECT_THROW(graph.setEdgeMultiplicityIdx(0, 1, 1), std::out_of_range);
}
