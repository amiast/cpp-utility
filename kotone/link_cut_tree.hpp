#ifndef KOTONE_LINK_CUT_TREE_HPP
#define KOTONE_LINK_CUT_TREE_HPP 1

#include <vector>
#include <cassert>

namespace kotone {

// A data structure for maintaining path aggregation in a forest.
// Optionally, provide the following functions to enable aggregation queries:
// - `void on_update(int parent, int left, int right)`
// - `void on_reverse(int parent, int left, int right)`
// - `void on_push(int parent, int left, int right)`
//
// Reference: https://usaco.guide/adv/link-cut-tree
// Reference: https://nyaannyaan.github.io/library/lct/link-cut-base.hpp
template <
    void (*on_update)(int, int, int) = nullptr,
    void (*on_reverse)(int, int, int) = nullptr,
    void (*on_push)(int, int, int) = nullptr
> struct link_cut_tree {
  private:
    struct splay_vertex {
        int parent = -1, left = -1, right = -1, size = 1;
        bool rev = false;
    };
    std::vector<splay_vertex> _vec;
    int _len = 0;

    void _update(int v) {
        int l = _vec[v].left, r = _vec[v].right;
        _vec[v].size = 1;
        if (l != -1) _vec[v].size += _vec[l].size;
        if (r != -1) _vec[v].size += _vec[r].size;
        if constexpr (on_update) on_update(v, l, r);
    }

    void _reverse(int v) {
        int &l = _vec[v].left, &r = _vec[v].right;
        if constexpr (on_reverse) on_reverse(v, l, r);
        std::swap(l, r);
        _vec[v].rev ^= true;
    }

    void _push(int v) {
        int l = _vec[v].left, r = _vec[v].right;
        if (_vec[v].rev) {
            if (l != -1) _reverse(l);
            if (r != -1) _reverse(r);
            _vec[v].rev = false;
        }
        if constexpr (on_push) on_push(v, l, r);
    }

    int _dir(int v) const noexcept {
        int p = _vec[v].parent;
        if (p == -1) return -2;
        if (v == _vec[p].left) return 0;
        if (v == _vec[p].right) return 1;
        return -1;
    }

    // Adds `v` as a splay child of `u`.
    void _merge(int p, int c, int d) noexcept {
        if (d == 0) _vec[p].left = c;
        else if (d == 1) _vec[p].right = c;
        if (c != -1) _vec[c].parent = p;
    }

    // Rotates the splay tree and moves `v` a step closer to the splay root.
    void _rotate(int v) {
        int d = _dir(v);
        int p = _vec[v].parent;
        _merge(_vec[p].parent, v, _dir(p));
        if (d == 0) _merge(p, _vec[v].right, 0);
        else _merge(p, _vec[v].left, 1);
        _merge(v, p, 1 - d);
        _update(p);
    }

    // Moves `v` to the root of its splay tree.
    void _splay(int v) {
        while (_dir(v) >= 0 && _dir(_vec[v].parent) >= 0) {
            _push(_vec[_vec[v].parent].parent);
            _push(_vec[v].parent);
            _push(v);
            if (_dir(v) == _dir(_vec[v].parent)) _rotate(_vec[v].parent);
            else _rotate(v);
            _rotate(v);
        }
        if (_dir(v) >= 0) {
            _push(_vec[v].parent);
            _push(v);
            _rotate(v);
        }
        _push(v);
        _update(v);
    }

    // Sets `root`-to-`v` as the preferred path and splays `v`.
    void _access(int v) {
        int p = v, c = -1;
        while (p != -1) {
            _splay(p);
            _vec[p].right = c;
            _update(p);
            c = p;
            p = _vec[p].parent;
        }
        _splay(v);
    }

    // Returns the level ancestor of `v` at depth `d`.
    int _jump(int v, int d) {
        _push(v);
        int l = _vec[v].left;
        int dv = l == -1 ? 0 : _vec[l].size;
        if (d == dv) {
            _splay(v);
            return v;
        }
        if (d < dv) return _jump(l, d);
        return _jump(_vec[v].right, d - dv - 1);
    }

  public:
    link_cut_tree() {}

    // Initializes a link-cut tree with the specified number of vertices.
    link_cut_tree(int num_vertices) : _len(num_vertices) {
        assert(num_vertices >= 0);
        _vec.resize(_len);
    }

    // Exposes the vertex `v`.
    // This method should be called before:
    // - Querying `root`-to-`v` path aggegation; or
    // - Modifying aggregation-related values of `v`.
    //
    // Requires `0 <= v < num_vertices`.
    void access(int v) {
        assert(0 <= v && v < _len);
        _access(v);
    }

    // Updates the vertex `v`.
    // This method should be called after modifying aggregation-related values of `v`.
    //
    // Requires `0 <= v < num_vertices`.
    void update(int v) {
        assert(0 <= v && v < _len);
        _update(v);
    }

    // Returns the root of the tree containing `v`.
    // Requires `0 <= v < num_vertices`.
    int root(int v) {
        assert(0 <= v && v < _len);
        _access(v);
        while (_vec[v].left != -1) {
            v = _vec[v].left;
            _push(v);
        }
        _splay(v);
        return v;
    }

    // Removes the edge between `v` and its parent.
    // Requires `0 <= v < num_vertices`.
    // Requires the parent to exist.
    void cut(int v) {
        assert(0 <= v && v < _len);
        _access(v);
        assert(_vec[v].left != -1);
        _vec[_vec[v].left].parent = -1;
        _vec[v].left = -1;
        _update(v);
    }

    // Adds `p` as the parent of `v`.
    // Requires `0 <= v < num_vertices`.
    // Requires `0 <= p < num_vertices`.
    // Requires `v` to be the root of a tree.
    // Requires `v` and `p` to be disconnected.
    void link(int v, int p) {
        assert(0 <= v && v < _len);
        assert(0 <= p && p < _len);
        _access(v);
        assert(_vec[v].left == -1);
        assert(v != root(p));
        _access(p);
        _vec[v].parent = p;
    }

    // Reorients the tree containing `v` such that `v` becomes the new root.
    // Requires `0 <= v < num_vertices`.
    void reorient(int v) {
        assert(0 <= v && v < _len);
        _access(v);
        _reverse(v);
        _access(v);
    }

    // Returns the lowest common ancestor of `u` and `v`.
    // If `u` and `v` are disconnected, returns `-1`.
    // Requires `0 <= u < num_vertices`.
    // Requires `0 <= v < num_vertices`.
    int lca(int u, int v) {
        assert(0 <= u && u < _len);
        assert(0 <= v && v < _len);
        if (u == v) return u;
        _access(u);
        _access(v);
        if (_vec[u].parent == -1) return -1;
        _splay(u);
        if (_vec[u].parent == -1) return u;
        return _vec[u].parent;
    }

    // Returns the number of edges between `v` and its root.
    // Requires `0 <= v < num_vertices`.
    int depth(int v) {
        assert(0 <= v && v < _len);
        _access(v);
        if (int l = _vec[v].left; l == -1) return 0;
        else return _vec[l].size;
    }

    // Returns the level ancestor of `v` at depth `d` relative to the root.
    // If the level ancestor does not exist, returns `-1`.
    // Requires `0 <= v < num_vertices`.
    // Requires `d >= 0`.
    int level_ancestor(int v, int d) {
        assert(0 <= v && v < _len);
        assert(d >= 0);
        if (d > depth(v)) return -1;
        return _jump(v, d);
    }
};

}  // namespace kotone

#endif  // KOTONE_LINK_CUT_TREE_HPP
