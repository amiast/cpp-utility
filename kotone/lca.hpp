#ifndef KOTONE_LCA_HPP
#define KOTONE_LCA_HPP 1

#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <kotone/sparse_table>

namespace kotone {

// A data structure that manages the lowest common ancestors (LCA) of nodes in a tree (forest).
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/linear_time_lca
struct lca_tree {
  private:
    constexpr static int _BLOCK_SIZE = 8;
    constexpr static std::array<std::array<std::array<int, _BLOCK_SIZE>, _BLOCK_SIZE>, 1 << (_BLOCK_SIZE - 1)> _TABLE = [](){
        std::array<int, _BLOCK_SIZE> arr{};
        std::array<std::array<std::array<int, _BLOCK_SIZE>, _BLOCK_SIZE>, 1 << (_BLOCK_SIZE - 1)> table{};
        for (int k = 0; k < 1 << (_BLOCK_SIZE - 1); k++) {
            for (int i = 0; i < _BLOCK_SIZE - 1; i++) arr[i + 1] = arr[i] + (k >> i & 1 ? -1 : 1);
            for (int l = 0; l < _BLOCK_SIZE; l++) {
                int j = l;
                for (int i = l; i < _BLOCK_SIZE; i++) {
                    if (arr[i] < arr[j]) j = i;
                    table[k][l][i] = j;
                }
            }
        }
        return table;
    }();

    kotone::sparse_table<std::pair<int, int>,
        [](std::pair<int, int> a, std::pair<int, int> b){ return (a < b ? a : b); }
    > _st;
    std::vector<int> _pos, _depth, _euler, _patterns;
    int _num_nodes = 0;

    void _euler_tour(const std::vector<std::vector<int>> &tree, int u, int p, int d) {
        _pos[u] = _euler.size();
        _depth[u] = d;
        _euler.push_back(u);
        for (int v : tree[u]) {
            if (v == p) continue;
            _euler_tour(tree, v, u, d + 1);
            _euler.push_back(u);
        }
    }

  public:
    lca_tree() {}

    // Constructs a tree for the specified edges.
    // Constructs a rooted forest if every edge `{u, v}` is directed from `u` to `v`.
    // Otherwise, constructs a forest with arbitrary roots.
    // Requires `num_nodes >= 0`.
    // Requires `0 <= u, v < num_nodes` for each pair `{u, v}` in `edges`.
    // Requires `edges` to represent a valid forest.
    lca_tree(int num_nodes, const std::vector<std::pair<int, int>> &edges) : _num_nodes(num_nodes) {
        assert(num_nodes >= 0);
        _pos.assign(num_nodes + 1, -1);
        _depth.resize(num_nodes + 1);
        {
            bool is_rooted = true;
            std::vector<bool> is_root(num_nodes, true);
            std::vector<std::vector<int>> tree(num_nodes);
            for (auto [u, v] : edges) {
                assert(0 <= u && u < num_nodes);
                assert(0 <= v && v < num_nodes);
                assert(u != v);
                is_rooted &= is_root[v];
                is_root[v] = false;
                tree[u].push_back(v);
                tree[v].push_back(u);
            }
            _pos[num_nodes] = 0;
            _depth[num_nodes] = -1;
            _euler.push_back(num_nodes);
            for (int i = 0; i < num_nodes; i++) {
                if (is_rooted && !is_root[i]) continue;
                if (!is_rooted && _pos[i] != -1) continue;
                _euler_tour(tree, i, i, 0);
                _euler.push_back(num_nodes);
            }
        }
        int euler_len = _euler.size(), st_len = (euler_len + _BLOCK_SIZE - 1) / _BLOCK_SIZE;
        _patterns.resize(st_len);
        std::vector<std::pair<int, int>> vec(st_len);
        for (int b = 0; b < st_len; b++) {
            int l = b * _BLOCK_SIZE;
            int r = std::min((b + 1) * _BLOCK_SIZE, euler_len);
            int j = l;
            for (int i = l; i < r; i++) {
                if (_depth[_euler[i]] < _depth[_euler[j]]) j = i;
                if (i + 1 < r && _depth[_euler[i + 1]] == _depth[_euler[i]] - 1) {
                    _patterns[b] |= 1 << (i - l);
                }
            }
            vec[b] = {_depth[_euler[j]], j};
        }
        _st = decltype(_st)(vec);
    }

    // Returns the number of nodes in the tree.
    int size() const noexcept {
        return _num_nodes;
    }

    // Returns the lowest common ancestor of nodes `u` and `v`.
    // If `u` and `v` are disconnected, returns `-1`.
    // Requires `0 <= u < num_nodes`.
    // Requires `0 <= v < num_nodes`.
    int lca(int u, int v) {
        assert(0 <= u && u < _num_nodes);
        assert(0 <= v && v < _num_nodes);
        int l = _pos[u], r = _pos[v];
        if (l > r) std::swap(l, r);
        int bl = l / _BLOCK_SIZE, br = r / _BLOCK_SIZE;
        if (bl == br) {
            int i = bl * _BLOCK_SIZE + _TABLE[_patterns[bl]][l % _BLOCK_SIZE][r % _BLOCK_SIZE];
            return _euler[i] == _num_nodes ? -1 : _euler[i];
        }
        int x = _euler[bl * _BLOCK_SIZE + _TABLE[_patterns[bl]][l % _BLOCK_SIZE][_BLOCK_SIZE - 1]];
        int y = _euler[br * _BLOCK_SIZE + _TABLE[_patterns[br]][0][r % _BLOCK_SIZE]];
        int w = _depth[x] < _depth[y] ? x : y;
        if (bl + 1 < br) {
            auto [d, i] = _st.prod(bl + 1, br);
            if (_depth[w] > d) w = _euler[i];
        }
        return w == _num_nodes ? -1 : w;
    }

    // Returns the distance between nodes `u` and `v`.
    // If `u` and `v` are disconnected, returns `-1`.
    // Requires `0 <= u < num_nodes`.
    // Requires `0 <= v < num_nodes`.
    int dist(int u, int v) {
        int w = lca(u, v);
        if (w == -1) return -1;
        return _depth[u] + _depth[v] - _depth[w] * 2;
    }
};

}  // namespace kotone

#endif  // KOTONE_LCA_HPP
