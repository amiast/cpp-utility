#ifndef KOTONE_HEAVY_LIGHT_DECOMPOSITION_HPP
#define KOTONE_HEAVY_LIGHT_DECOMPOSITION_HPP 1

#include <vector>
#include <cassert>

namespace kotone {

// Represents an undirected tree as a partition into directed paths.
struct heavy_light_decomposition_tree {
  private:
    std::vector<std::vector<int>> _tree;
    std::vector<int> _size, _parent, _head, _order;
    int _num_nodes = 0, _root = -1;

    void _init_reorient() {
        _size.assign(_num_nodes, 1);
        _parent.assign(_num_nodes, -1);
        auto go = [&](auto &go, int u) -> void {
            for (int &v : _tree[u]) {
                _parent[v] = u;
                std::erase(_tree[v], u);
                go(go, v);
                _size[u] += _size[v];
                if (_size[v] > _size[_tree[u][0]]) std::swap(v, _tree[u][0]);
            }
        };
        go(go, _root);
    }

    void _init_ordering() {
        _head.assign(_num_nodes, _root);
        _order.resize(_num_nodes);
        int time = 0;
        auto go = [&](auto &go, int u) -> void {
            _order[u] = time++;
            for (int v : _tree[u]) {
                if (v == _tree[u][0]) _head[v] = _head[u];
                else _head[v] = v;
                go(go, v);
            }
        };
        go(go, _root);
    }

  public:
    heavy_light_decomposition_tree() {}

    // Constructs a rooted tree from the given undirected edges.
    // Requires `edges` to be a valid representation of an undirected tree.
    // Requires `0 <= u, v < num_nodes` for each edge `{u, v}`, where `num_nodes == edges.size() + 1`.
    // Requires `0 <= root < num_nodes`.
    heavy_light_decomposition_tree(const std::vector<std::pair<int, int>> &edges, int root = 0) {
        int m = edges.size();
        assert(0 <= root && root <= m);
        _num_nodes = m + 1;
        _root = root;
        _tree.resize(_num_nodes);
        for (auto [u, v] : edges) {
            assert(0 <= u && u <= m);
            assert(0 <= v && v <= m);
            _tree[u].push_back(v);
            _tree[v].push_back(u);
        }
        _init_reorient();
        _init_ordering();
    }

    // Returns the number of nodes in the subtree rooted at `v`.
    // Requires `0 <= v < num_nodes`.
    int size(int v) const {
        assert(0 <= v && v < _num_nodes);
        return _size[v];
    }

    // Returns the parent of `v` in the tree.
    // If `v` is the root, returns `-1`.
    // Requires `0 <= v < num_nodes`.
    int parent(int v) const {
        assert(0 <= v && v < _num_nodes);
        return _parent[v];
    }

    // Returns the head of the subpath containing node `v` in the tree.
    // Requires `0 <= v < num_nodes`.
    int head(int v) const {
        assert(0 <= v && v < _num_nodes);
        return _head[v];
    }

    // Returns the DFS pre-order position of node `v` in the tree.
    // Requires `0 <= v < num_nodes`.
    int order(int v) const {
        assert(0 <= v && v < _num_nodes);
        return _order[v];
    }

    // Returns the path from `u` to `v` as a sequence of directed subpaths.
    // Each `s`-`t` subpath is represented as a pair `{s, t}`.
    // Requires `0 <= u, v < num_nodes`.
    std::vector<std::pair<int, int>> to_subpaths(int u, int v) const {
        assert(0 <= u && u < _num_nodes);
        assert(0 <= v && v < _num_nodes);
        std::vector<std::pair<int, int>> front, back;
        while (_head[u] != _head[v]) {
            if (_order[u] > _order[v]) {
                front.emplace_back(u, _head[u]);
                u = _parent[_head[u]];
            } else {
                back.emplace_back(_head[v], v);
                v = _parent[_head[v]];
            }
        }
        front.emplace_back(u, v);
        front.insert(front.end(), back.rbegin(), back.rend());
        return front;
    }
};

}  // namespace kotone

#endif  // KOTONE_HEAVY_LIGHT_DECOMPOSITION_HPP
