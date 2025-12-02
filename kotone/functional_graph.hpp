#ifndef KOTONE_FUNCTIONAL_GRAPH_HPP
#define KOTONE_FUNCTIONAL_GRAPH_HPP 1

#include <vector>
#include <cassert>

namespace kotone {

// A representation of a functional graph.
// - `in_cycle[i]` represents whether node `i` is contained in a cycle.
// - `cycles` is a vector of cycles, each represented as a vector of nodes.
//   Nodes are rearranged in the order visited from an arbitrary node in the cycle.
//   The order of cycles is undefined.
// - `forest` is the adjacency list of the graph obtained by removing cycles and reversing the remaining edges.
// - `size` is the number of nodes.
struct functional_graph {
    std::vector<bool> in_cycle;
    std::vector<std::vector<int>> cycles, forest;
    int size;
};

// Parses the adjacency vector and returns a corresponding `functional_graph` object.
// Requires `0 <= adjacency[i] < adjacency.size()` for all `i`.
functional_graph parse_functional_graph(const std::vector<int> &adjacency) {
    int size = adjacency.size();
    for (int i = 0; i < size; i++) {
        assert(0 <= adjacency[i] && adjacency[i] < size);
    }
    std::vector<int> visited(size, -1);
    std::vector<bool> in_cycle(size);
    std::vector<std::vector<int>> cycles;
    for (int i = 0; i < size; i++) {
        if (visited[i] != -1) continue;
        int u = i;
        while (visited[u] == -1) {
            visited[u] = i;
            u = adjacency[u];
        }
        if (visited[u] != i) continue;
        cycles.emplace_back();
        while (!in_cycle[u]) {
            in_cycle[u] = true;
            cycles.back().push_back(u);
            u = adjacency[u];
        }
    }
    std::vector<std::vector<int>> forest(size);
    for (int i = 0; i < size; i++) {
        if (in_cycle[i]) continue;
        forest[adjacency[i]].push_back(i);
    }
    return {in_cycle, cycles, forest, size};
}

}  // namespace kotone

#endif  // KOTONE_FUNCTIONAL_GRAPH_HPP
