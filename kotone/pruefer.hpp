#ifndef KOTONE_PRUEFER_HPP
#define KOTONE_PRUEFER_HPP

#include <vector>
#include <queue>
#include <cassert>

namespace kotone {

// Returns the Pruefer code of the specified undirected tree.
// Requires `tree` to be a valid representation of an undirected tree.
// Requires `tree.size() >= 2`.
// Requires `0 <= tree[i][j] < tree.size()` for all `0 <= i < tree.size()` and `0 <= j < tree[i].size()`.
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/prufer_code
std::vector<int> encode_pruefer(const std::vector<std::vector<int>> &tree) {
    assert(int(tree.size()) >= 2);
    int n = tree.size();
    std::vector<int> deg(n);
    for (int i = 0; i < n; i++) deg[i] = tree[i].size();
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < n; i++) if (deg[i] == 1) pq.push(i);
    std::vector<int> result;
    for (int i = 0; i < n - 2; i++) {
        int u = pq.top();
        pq.pop();
        deg[u] = 0;
        for (int v : tree[u]) {
            assert(0 <= v && v < n);
            if (deg[v] == 0) continue;
            result.push_back(v);
            if (--deg[v] == 1) pq.push(v);
        }
    }
    return result;
}

// Returns the adjacency list of the undirected tree corresponding to the specified Pruefer code.
// The order of vertices in each list is undefined.
// Requires `code` to be a valid representation of a Pruefer code.
// Requires `0 <= code[i] < code.size() + 2`.
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/prufer_code
std::vector<std::vector<int>> decode_pruefer(const std::vector<int> &code) {
    int n = code.size() + 2;
    std::vector<int> count(n);
    for (int p : code) {
        assert(0 <= p && p < n);
        count[p]++;
    }
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
    for (int i = 0; i < n; i++) if (count[i] == 0) pq.push(i);
    std::vector<std::vector<int>> tree(n);
    for (int i = 0; i < n - 2; i++) {
        int u = pq.top();
        pq.pop();
        int v = code[i];
        tree[u].push_back(v);
        tree[v].push_back(u);
        if (--count[v] == 0) pq.push(v);
    }
    int u = pq.top();
    pq.pop();
    int v = pq.top();
    tree[u].push_back(v);
    tree[v].push_back(u);
    return tree;
}

}  // namespace kotone

#endif  // KOTONE_PRUEFER_HPP
