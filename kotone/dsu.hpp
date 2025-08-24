#ifndef KOTONE_DSU_HPP
#define KOTONE_DSU_HPP 1

#include <vector>
#include <cassert>
#include <algorithm>

namespace kotone {

// A basic data structure that monitors connectivity in a graph.
// Reference: AtCoder Library
struct dsu {
  private:
    int _num_nodes;
    std::vector<int> _parent_or_size;

  public:
    dsu(int num_nodes) : _num_nodes(num_nodes), _parent_or_size(num_nodes, -1) {
        assert(num_nodes >= 0);
        assert(num_nodes <= 100000000);
    }

    // Returns the leader of the connected component containing `v`.
    int leader(int v) {
        assert(v >= 0);
        assert(v < _num_nodes);
        if (_parent_or_size[v] < 0) return v;
        return _parent_or_size[v] = leader(_parent_or_size[v]);
    }

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    virtual int merge(int u, int v) {
        u = leader(u);
        v = leader(v);
        if (u == v) return u;
        if (_parent_or_size[u] > _parent_or_size[v]) std::swap(u, v);
        _parent_or_size[u] += _parent_or_size[v];
        _parent_or_size[v] = u;
        return u;
    }

    // Returns whether `u` and `v` belong to the same connected component.
    bool same(int u, int v) {
        return leader(u) == leader(v);
    }

    // Returns the size of the connected component containing `v`.
    int size(int v) {
        return -_parent_or_size[leader(v)];
    }
};

// An extended DSU with internal mapping between connected components and monoids.
template <typename S, S (*op)(S, S), S (*e)()> struct extended_dsu : dsu {
  private:
    std::vector<S> _vec;
  public:
    extended_dsu(int num_nodes) : dsu(num_nodes), _vec(num_nodes, e()) {}

    extended_dsu(const std::vector<S> &vec) : dsu(vec.size()), _vec(vec) {}

    // Merges the connected components containing `u` and `v`,
    // then returns the leader of the merged component.
    // Also merges the associated monoids.
    int merge(int u, int v) override {
        u = leader(u);
        v = leader(v);
        if (same(u, v)) return u;
        S result = op(_vec[u], _vec[v]);
        _vec[dsu::merge(u, v)] = std::move(result);
        return leader(u);
    }

    // Returns a copy of the monoid associated with the connected component containing `v`.
    S get(int v) {
        return _vec[leader(v)];
    }

    // Sets `x` as the monoid associated with the connected component containing `v`.
    void set(int v, S x) {
        _vec[leader(v)] = std::move(x);
    }
};

}  // namespace kotone

#endif  // KOTONE_DSU_HPP
