#ifndef KOTONE_REROOTING_HPP
#define KOTONE_REROOTING_HPP 1

#include <vector>
#include <cassert>
#include <kotone/dsu>

namespace kotone {

// Maintains dynamic programming for monoids at different roots of trees in a forest.
// Requires the following functions:
// - `S merge(S dp_l, S dp_r)`
// - `S apply(S dp_child, int child, int parent)`
// - `S identity()`
template <typename S, S (*merge)(S, S), S (*apply)(S, int, int), S (*identity)()> struct rerooting {
  private:
    int _size = 0;
    std::vector<std::vector<int>> _tree;
    std::vector<S> _dp;
    kotone::dsu<int> _dsu;

    void _dfs_forward(int u, int p) {
        _dp[u] = identity();
        for (int v : _tree[u]) {
            if (v == p) continue;
            _dfs_forward(v, u);
            _dp[u] = merge(_dp[u], apply(_dp[v], v, u));
        }
    }

    void _dfs_reverse(int u, int p, S parent_acc, std::vector<S> &result) {
        int deg = static_cast<int>(_tree[u].size());
        std::vector<S> prefix(deg + 1, identity()), suffix(deg + 1, identity());
        for (int i = 0; i < deg; i++) {
            int v = _tree[u][i];
            if (v == p) prefix[i + 1] = merge(prefix[i], apply(parent_acc, p, u));
            else prefix[i + 1] = merge(prefix[i], apply(_dp[v], v, u));
        }
        for (int i = deg - 1; i >= 0; i--) {
            int v = _tree[u][i];
            if (v == p) suffix[i] = merge(suffix[i + 1], apply(parent_acc, p, u));
            else suffix[i] = merge(suffix[i + 1], apply(_dp[v], v, u));
        }
        result[u] = prefix[deg];
        for (int i = 0; i < deg; i++) {
            int v = _tree[u][i];
            if (v == p) continue;
            S acc = merge(prefix[i], suffix[i + 1]);
            _dfs_reverse(v, u, acc, result);
        }
    }

  public:
    rerooting() {}

    // Constructs a forest with the specified number of disconnected nodes.
    // Requires `0 <= num_nodes <= 100000000`.
    rerooting(int num_nodes) : _size(num_nodes), _dsu(num_nodes) {
        assert(0 <= num_nodes && num_nodes <= 100000000);
        _tree.resize(num_nodes);
        _dp.resize(num_nodes);
    }

    // Returns the number of nodes in the forest.
    int size() const {
        return _size;
    }

    // Adds an edge between nodes `u` and `v`.
    // Requires `0 <= u, v < size()`.
    // Requires `u` and `v` to be formerly disconnected.
    void add_edge(int u, int v) {
        assert(0 <= u && u < _size);
        assert(0 <= v && v < _size);
        assert(!_dsu.connected(u, v));
        _tree[u].push_back(v);
        _tree[v].push_back(u);
        _dsu.merge(u, v);
    }

    // Evaluates and returns a vector of the function at different roots of trees in the forest.
    std::vector<S> evaluate() {
        std::vector<S> result(_size);
        for (int i = 0; i < _size; i++) {
            if (i != _dsu.leader(i)) continue;
            _dfs_forward(i, -1);
            _dfs_reverse(i, -1, identity(), result);
        }
        return result;
    }
};

}  // namespace kotone

#endif  // KOTONE_REROOTING_HPP
