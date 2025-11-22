#ifndef KOTONE_LCA_HPP
#define KOTONE_LCA_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>
#include <kotone/dsu>

namespace kotone {

// A data structure that manages the lowest common ancestors (LCA) of nodes in a tree (forest).
struct lca_tree {
  private:
    int _num_nodes = 0;
    std::vector<std::vector<int>> _tree, _sparse_table;
    std::vector<int> _depth, _euler, _level, _first, _log, _is_root;
    dsu<int> _dsu;
    bool _requires_build = false;
    bool _is_rooted = true;

    void _euler_tour(int u, int p, int d) {
        _depth[u] = d;
        _first[u] = _euler.size();
        _euler.push_back(u);
        _level.push_back(d);
        for (int v : _tree[u]) {
            if (v == p) continue;
            _euler_tour(v, u, d + 1);
            _euler.push_back(u);
            _level.push_back(d);
        }
    }

    void _build() {
        if (!_requires_build) return;
        _requires_build = false;
        _euler.clear();
        _level.clear();
        for (int i = 0; i < _num_nodes; i++) {
            if (
                (_is_rooted && _is_root[i])
                || (!_is_rooted && i == _dsu.leader(i))
            ) _euler_tour(i, -1, 0);
        }
        int M = _euler.size();
        _log.resize(M + 1);
        for (int i = 2; i <= M; i++) _log[i] = _log[i / 2] + 1;
        int K = _log[M] + 1;
        _sparse_table.assign(K, std::vector<int>(M));
        for (int i = 0; i < M; i++) _sparse_table[0][i] = i;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i + (1 << k) <= M; i++) {
                int x = _sparse_table[k - 1][i];
                int y = _sparse_table[k - 1][i + (1 << (k - 1))];
                _sparse_table[k][i] = _level[x] < _level[y] ? x : y;
            }
        }
    }

  public:
    lca_tree() {}

    // Constructs a tree for the specified number of nodes.
    lca_tree(int num_nodes) : _num_nodes(num_nodes), _dsu(num_nodes) {
        assert(0 <= num_nodes && num_nodes <= 100000000);
        _tree.resize(num_nodes);
        _depth.resize(num_nodes);
        _first.resize(num_nodes);
        _is_root.resize(num_nodes, 1);
    }

    // Returns the number of nodes in the tree.
    int size() const noexcept {
        return _num_nodes;
    }

    // Adds an edge between the specified parent and child nodes.
    // If `child` already has an assigned parent, the tree becomes undirected.
    // - This alters the behavior of `get_lca`.
    //
    // Requires `parent` and `child` to be formerly disconnected.
    // Requires `0 <= parent < num_nodes`.
    // Requires `0 <= child < num_nodes`.
    void add_edge(int parent, int child) {
        assert(0 <= parent && parent < _num_nodes);
        assert(0 <= child && child < _num_nodes);
        assert(!_dsu.connected(parent, child));
        if (!_is_root[child]) _is_rooted = false;
        _tree[parent].push_back(child);
        _tree[child].push_back(parent);
        _dsu.merge(parent, child);
        _is_root[child] = false;
        _requires_build = true;
    }

    // Prompts the tree to build the LCA table immediately.
    void build() {
        _build();
    }

    // Returns the lowest common ancestor of nodes `u` and `v`.
    // If `u` and `v` are disconnected, returns `-1`.
    // If the tree is undirected due to the effect of `add_edge`,
    // the LCA is assigned to be an arbitrary node in the tree (or the connected component in a forest).
    // Requires `0 <= u < num_nodes`.
    // Requires `0 <= v < num_nodes`.
    int get_lca(int u, int v) {
        assert(0 <= u && u < _num_nodes);
        assert(0 <= v && v < _num_nodes);
        if (_requires_build) _build();
        if (!_dsu.connected(u, v)) return -1;
        int L = _first[u], R = _first[v];
        if (L > R) std::swap(L, R);
        int k = _log[R - L + 1];
        int x = _sparse_table[k][L];
        int y = _sparse_table[k][R - (1 << k) + 1];
        return _level[x] < _level[y] ? _euler[x] : _euler[y];
    }

    // Returns the distance between nodes `u` and `v`.
    // If `u` and `v` are disconnected, returns `-1`.
    // Requires `0 <= u < num_nodes`.
    // Requires `0 <= v < num_nodes`.
    int get_distance(int u, int v) {
        int w = get_lca(u, v);
        if (w == -1) return -1;
        return _depth[u] + _depth[v] - _depth[w] * 2;
    }
};

} // namespace kotone

#endif  // KOTONE_LCA_HPP
