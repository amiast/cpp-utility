/**
 * max_flow.cpp
 */

#include "max_flow.h"
#include <queue>

bool BFS(std::vector<std::vector<int>> &residual_graph, int s, int t, std::vector<int> &parent) {
    std::vector<bool> visited(residual_graph.size());
    std::queue<int> queue;
    queue.push(s);
    visited.at(s) = true;

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        for (int v = 0; v < residual_graph.size(); v++) {
            if (visited.at(v) || residual_graph.at(u).at(v) <= 0) continue;

            parent.at(v) = u;
            if (v == t) return true;

            queue.push(v);
            visited.at(v) = true;
        }
    }

    return false;
}

std::pair<std::vector<std::vector<int>>, int> max_flow(std::vector<std::vector<int>> &graph, int s, int t) {
    auto residual_graph = graph;
    std::vector result(graph.size(), std::vector(graph.size(), 0));
    std::vector parent(graph.size(), -1);
    int flow = 0;

    while (BFS(residual_graph, s, t, parent)) {
        int this_flow = residual_graph.at(parent.at(t)).at(t);
        for (int v = parent.at(t); v != s; v = parent.at(v)) {
            int u = parent.at(v);
            if (residual_graph.at(u).at(v) < this_flow) {
                this_flow = residual_graph.at(u).at(v);
            }
        }

        for (int v = t; v != s; v = parent.at(v)) {
            int u = parent.at(v);
            residual_graph.at(u).at(v) -= this_flow;
            residual_graph.at(v).at(u) += this_flow;
            result.at(u).at(v) += this_flow;
        }

        flow += this_flow;
    }

    return {result, flow};
}
