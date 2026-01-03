#ifndef KOTONE_PROJECT_SELECTION_HPP
#define KOTONE_PROJECT_SELECTION_HPP 1

#include <atcoder/maxflow>

namespace kotone {

// A minimal wrapper for solving project selection problems using flow network algorithm.
template <typename Cap> struct project_selection {
    atcoder::mf_graph<Cap> graph;
    int source, sink;

    // Constructs a flow network for the specified number of projects.
    project_selection(int num_projects) : graph(num_projects + 2), source(num_projects), sink(num_projects + 1) {}

    // Adds the specified cost if project `i` is assigned `b`.
    // Requires `0 <= i < num_projects`.
    // Requires `cost >= 0`.
    void add_single(int i, bool b, Cap cost) {
        if (b) graph.add_edge(i, sink, cost);
        else graph.add_edge(source, i, cost);
    }

    // Adds the specified cost if project `i` is assigned `true` and project `j` is assigned `false`.
    // Requires `0 <= i, j < num_projects`.
    // Requires `cost >= 0`.
    void add_pair(int i, int j, Cap cost) {
        graph.add_edge(i, j, cost);
    }

    // Finds an optimal assignment and returns the minimum cost.
    // This function should be called once after adding constraints to the flow network.
    Cap eval_cost() {
        return graph.flow(source, sink);
    }

    // Returns a vector of `bool` indicating the assignment of each project.
    // This function should be called after `eval_cost`.
    std::vector<bool> assignment() {
        std::vector<bool> result = graph.min_cut(source);
        result.pop_back();
        result.pop_back();
        return result;
    }
};

}  // namespace kotone

#endif  // KOTONE_PROJECT_SELECTION_HPP
