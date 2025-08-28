#ifndef KOTONE_DSU_HPP
#define KOTONE_DSU_HPP 1

#include <vector>
#include <cassert>
#include <algorithm>

namespace kotone {

// A basic data structure that monitors connectivity in a graph.
// Optionally monitors the potential differences between nodes.
// Reference: AtCoder Library
template <typename T = int> struct dsu {
  protected:
    int _num_nodes;
    bool _defines_pd = true;
    std::vector<int> _parent_or_size;
    std::vector<T> _p;

    T _potential(int v) {
        leader(v);
        return _p[v];
    }

  public:
    dsu() : _num_nodes(0) {}

    // Creates a graph with the specified `num_nodes` and no edges.
    dsu(int num_nodes) : _num_nodes(num_nodes), _parent_or_size(num_nodes, -1), _p(num_nodes) {
        assert(num_nodes >= 0);
        assert(num_nodes <= 100000000);
    }

    // Returns the leader of the connected component containing `v`.
    int leader(int v) {
        assert(v >= 0);
        assert(v < _num_nodes);
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
    // Requires all previous `merge()` calls to define potential differences.
    T potential_diff(int u, int v) {
        assert(connected(u, v));
        assert(_defines_pd);
        return _potential(v) - _potential(u);
    };

    // Adds an edge between `u` and `v`,
    // then returns the leader of the merged component.
    virtual int merge(int u, int v) {
        _defines_pd = false;
        return merge(u, v, T{});
    }

    // Adds an edge between `u` and `v`,
    // then returns the leader of the merged component.
    // If `u` and `v` are not formerly connected,
    // defines `pd` as the potential difference from `u` to `v`.
    virtual int merge(int u, int v, T pd) {
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
        return u;
    }

    // Returns the size of the connected component containing `v`.
    int size(int v) {
        return -_parent_or_size[leader(v)];
    }

    // Returns a vector of connected components as vectors of node indices.
    // The order of components is undefined.
    // Node indices in each group's vector appear in ascending order.
    std::vector<std::vector<int>> components() {
        std::vector<std::vector<int>> temp(_num_nodes), result;
        for (int i = 0; i < _num_nodes; i++) temp[leader(i)].emplace_back(i);
        for (int i = 0; i < _num_nodes; i++) {
            if (temp[i].size()) {
                result.push_back(std::move(temp[i]));
            }
        }
        return result;
    }
};

// An extended DSU with internal mapping between connected components and a semigroup.
// Optionally monitors the potential differences between nodes.
template <typename S, S (*op)(S, S), typename T = int> struct extended_dsu : dsu<T> {
  protected:
    std::vector<S> _map;

  public:
    extended_dsu() : dsu<T>() {}

    // Creates a graph with the specified `num_nodes` and no edges.
    // Each connected component is mapped to a value-initialized instance of `S`.
    extended_dsu(int num_nodes) : dsu<T>(num_nodes), _map(num_nodes) {}

    // Creates a graph with the specified `num_nodes` and no edges.
    // Each connected component is mapped to a copy of `init_x`.
    extended_dsu(int num_nodes, S init_x) : dsu<T>(num_nodes), _map(num_nodes, init_x) {}

    // Creates a graph with no edges.
    // For all `v`, maps connected component containing `v` to `vec[v]`.
    extended_dsu(const std::vector<S> &vec) : dsu<T>(vec.size()), _map(vec) {}

    // Adds an edge between `u` and `v`,
    // then returns the leader of the merged component.
    // Also merges their images under the mapping.
    int merge(int u, int v) override {
        this->_defines_pd = false;
        return merge(u, v, T{});
    }

    // Adds an edge between `u` and `v`,
    // then returns the leader of the merged component.
    // Also merges their images under the mapping.
    // If `u` and `v` are not formerly connected,
    // defines `pd` as the potential difference from `u` to `v`.
    int merge(int u, int v, T pd) override {
        if (this->connected(u, v)) return this->leader(u);
        S result = op(_map[this->leader(u)], _map[this->leader(v)]);
        _map[dsu<T>::merge(u, v, pd)] = std::move(result);
        return this->leader(u);
    }

    // Returns a copy of the image mapped from the connected component containing `v`.
    S get(int v) {
        return _map[this->leader(v)];
    }

    // Reassigns `x` as the image mapped from the connected component containing `v`,
    // then returns the leader of the modified component.
    int set(int v, S x) {
        v = this->leader(v);
        _map[v] = std::move(x);
        return v;
    }
};

}  // namespace kotone

#endif  // KOTONE_DSU_HPP
