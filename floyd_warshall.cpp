/**
 * floyd_warshall.cpp
 */

#include "floyd_warshall.h"
#include <climits>

void floyd_warshall(std::vector<std::vector<int64_t>> &graph, int64_t inf = LLONG_MAX) {
    for (int v = 0; v < graph.size(); v++) {
        graph.at(v).at(v) = 0;
    }

    for (int k = 0; k < graph.size(); k++) {
        for (int u = 0; u < graph.size(); u++) {
            if (graph.at(u).at(k) >= inf) continue;

            for (int v = 0; v < graph.size(); v++) {
                if (graph.at(k).at(v) >= inf) continue;

                if (graph.at(u).at(v) > graph.at(u).at(k) + graph.at(k).at(v)) {
                    graph.at(u).at(v) = graph.at(u).at(k) + graph.at(k).at(v);
                }
            }
        }
    }
}
