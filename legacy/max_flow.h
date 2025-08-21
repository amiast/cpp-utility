/**
 * max_flow.h
 */

#ifndef MAX_FLOW_H
#define MAX_FLOW_H

#include <vector>
#include <utility>

std::pair<std::vector<std::vector<int>>, int> max_flow(std::vector<std::vector<int>> &graph, int s, int t);

#endif
