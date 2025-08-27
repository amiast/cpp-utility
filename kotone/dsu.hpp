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
        _p[v] += _p[_parent_or_size[v]];
        return _parent_or_size[v] = l;
    }

    // Returns whether `u` and `v` belong to the same connected component.
    bool same(int u, int v) {
        return leader(u) == leader(v);
    }

    // Returns the potentital difference from `u` to `v`.
    // If potential differences between nodes are undefined,
    // the behavior is undefined as well.
    // Requires `u` and `v` to be in the same connected component.
    T potential_diff(int u, int v) {
        assert(same(u, v));
        return _potential(v) - _potential(u);
    };

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    virtual int merge(int u, int v) {
        return merge(u, v, 0);
    }

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    // If `u` and `v` are not formerly connected,
    // defines `pd` as the potential difference from `u` to `v`.
    virtual int merge(int u, int v, T pd) {
        if (same(u, v)) return u;
        pd += _potential(u) - _potential(v);
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

// An extended DSU with internal mapping between connected components and monoids.
// Optionally monitors the potential differences between nodes.
template <typename S, S (*op)(S, S), typename T = int> struct extended_dsu : dsu<T> {
  protected:
    std::vector<S> _vec;

  public:
    extended_dsu() : dsu<T>() {}

    // Creates a graph with the specified `num_nodes` and no edges.
    // Each node is associated with a value-initialized monoid.
    extended_dsu(int num_nodes) : dsu<T>(num_nodes), _vec(num_nodes) {}

    // Creates a graph with the specified `num_nodes` and no edges.
    // Each node is associated with a copy of `init_x`.
    extended_dsu(int num_nodes, S init_x) : dsu<T>(num_nodes), _vec(num_nodes, init_x) {}

    // Creates a graph with no edges.
    // Uses `vec` to initialize the monoids associated with nodes.
    extended_dsu(const std::vector<S> &vec) : dsu<T>(vec.size()), _vec(vec) {}

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    // Also merges the associated monoids.
    int merge(int u, int v) override {
        return merge(u, v, 0);
    }

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    // Also merges the associated monoids.
    // If `u` and `v` are not formerly connected,
    // defines `pd` as the potential difference from `u` to `v`.
    int merge(int u, int v, T pd) override {
        if (this->same(u, v)) return u;
        S result = op(_vec[this->leader(u)], _vec[this->leader(v)]);
        _vec[dsu<T>::merge(u, v, pd)] = std::move(result);
        return this->leader(u);
    }

    // Returns a copy of the monoid associated with the connected component containing `v`.
    S get(int v) {
        return _vec[this->leader(v)];
    }

    // Sets `x` as the monoid associated with the connected component containing `v`,
    // then returns the leader of the modified component.
    int set(int v, S x) {
        v = this->leader(v);
        _vec[v] = std::move(x);
        return v;
    }
};

}  // namespace kotone

#endif  // KOTONE_DSU_HPP
