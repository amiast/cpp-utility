#ifndef KOTONE_DSU_HPP
#define KOTONE_DSU_HPP 1

#include <vector>
#include <cassert>
#include <algorithm>
#include <kotone/internal_type_traits>

namespace kotone {

// A basic data structure that monitors connectivity in a graph.
// Optionally monitors the potential differences between nodes.
// Pass the following functions to monitor connected components with commutative monoids:
// - `void on_init(int node)`
// - `void on_merge(int new_leader, int old_leader)`
//
// Reference: AtCoder Library
template <
    additive pd_type = int,
    void (*on_init)(int) = nullptr,
    void (*on_merge)(int, int) = nullptr
> struct dsu {
  private:
    std::vector<int> _parent_or_size;
    std::vector<pd_type> _p;

    pd_type _potential(int v) {
        leader(v);
        return _p[v];
    }

  public:
    // Initializes DSU with an empty graph.
    dsu() {}

    // Initializes DSU with the specified `num_nodes` and no edges.
    // If provided, calls `on_init(v)` for each node `v`.
    // Requires `num_nodes >= 0`.
    dsu(int num_nodes) {
        assert(num_nodes >= 0);
        _parent_or_size.assign(num_nodes, -1);
        _p.resize(num_nodes);
        if constexpr (on_init) for (int v = 0; v < num_nodes; v++) on_init(v);
    }

    // Returns the number of nodes.
    int num_nodes() const noexcept {
        return _parent_or_size.size();
    }

    // Creates a new vertex and returns its index.
    // If provided, calls `on_init(v)` for the new node `v`.
    int add_vertex() {
        int v = num_nodes();
        _parent_or_size.push_back(-1);
        _p.emplace_back();
        if constexpr (on_init) on_init(v);
        return v;
    }

    // Returns the leader of the connected component containing `v`.
    // Requires `0 <= v < num_nodes`.
    int leader(int v) {
        assert(0 <= v && v < num_nodes());
        if (_parent_or_size[v] < 0) return v;
        int l = leader(_parent_or_size[v]);
        _p[v] = _p[v] + _p[_parent_or_size[v]];
        return _parent_or_size[v] = l;
    }

    // Returns whether `u` and `v` belong to the same connected component.
    bool connected(int u, int v) {
        return leader(u) == leader(v);
    }

    // Returns the potential difference from `u` to `v`.
    // Requires `u` and `v` to be connected.
    pd_type potential_diff(int u, int v) {
        assert(connected(u, v));
        return _potential(v) - _potential(u);
    };

    // Adds an edge between `u` and `v`,
    // then returns the leader of the merged component.
    // If `u` and `v` are disconnected, sets `pd` as the potential difference from `u` to `v`.
    // Chooses the new leader via union by size.
    // Calls `on_merge` if provided.
    int merge(int u, int v, pd_type pd = {}) {
        if (connected(u, v)) return leader(u);
        pd = pd + _potential(u) - _potential(v);
        u = leader(u);
        v = leader(v);
        if (_parent_or_size[u] > _parent_or_size[v]) {
            std::swap(u, v);
            pd = -pd;
        }
        _parent_or_size[u] += _parent_or_size[v];
        _parent_or_size[v] = u;
        _p[v] = pd;
        if constexpr (on_merge) on_merge(u, v);
        return u;
    }

    // Returns the size of the connected component containing `v`.
    int size(int v) {
        return -_parent_or_size[leader(v)];
    }

    // Returns a vector of connected components as vectors of node indices.
    // The order of components is undefined.
    // Each component contains member nodes in ascending order.
    std::vector<std::vector<int>> components() {
        int n = num_nodes();
        std::vector<std::vector<int>> temp(n), result;
        for (int i = 0; i < n; i++) temp[leader(i)].emplace_back(i);
        for (int i = 0; i < n; i++) {
            if (temp[i].size()) {
                result.push_back(std::move(temp[i]));
            }
        }
        return result;
    }
};

}  // namespace kotone

#endif  // KOTONE_DSU_HPP
