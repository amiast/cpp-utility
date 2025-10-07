#ifndef KOTONE_LINK_CUT_TREE_HPP
#define KOTONE_LINK_CUT_TREE_HPP 1

#include <vector>
#include <algorithm>
#include <concepts>
#include <cassert>

namespace kotone {

template <typename node> concept compatible_node = (
    std::constructible_from<node, int>
    && requires(node n) {
        { n.parent }        -> std::convertible_to<node*>;
        { n.light }         -> std::convertible_to<node*>;
        { n.heavy }         -> std::convertible_to<node*>;
        { n.lazy_reverse }  -> std::convertible_to<bool>;
        { n.index }         -> std::convertible_to<int>;
    }
);

// A minimal node that satisfies `compatible_node` for link-cut trees.
struct link_cut_node {
    link_cut_node *parent = nullptr, *light = nullptr, *heavy = nullptr;
    bool lazy_reverse = false;
    int index;
    link_cut_node(int node_index) : index(node_index) {}
};

// The base struct for link-cut trees.
template <typename derived_tree, compatible_node node> struct link_cut_tree_base {
  private:
    derived_tree& derived() { return static_cast<derived_tree&>(*this); }
    const derived_tree& derived() const { return static_cast<const derived_tree&>(*this); }

  protected:
    std::vector<node> _nodes;

    // Pushes all lazy operations from node `n` to its children.
    // Used to maintain correct subtree structure.
    // Can be redefined in derived classes for additional lazy behavior.
    void _push(node *n) {
        if (!n || !n->lazy_reverse) return;
        std::swap(n->light, n->heavy);
        if (n->light) n->light->lazy_reverse ^= true;
        if (n->heavy) n->heavy->lazy_reverse ^= true;
        n->lazy_reverse = false;
    }

    // Recomputes augmented data for node `n` based on its light and heavy children.
    // Called after any structural changes involving node `n`.
    // Can be redefined in derived classes to maintain information of subtrees.
    void _update(node *n) {}

    // Returns whether node `n` is currently the root of a splay tree.
    bool _is_root(node *n) const {
        return (
            !n->parent ||
            (n->parent->light != n && n->parent->heavy != n)
        );
    }

    // Rotates the splay tree to move `n` one level closer to the root.
    void _rotate(node *n) {
        node *parent = n->parent;
        node *ancestor = parent->parent;
        if (!_is_root(parent)) {
            if (ancestor->light == parent) ancestor->light = n;
            else ancestor->heavy = n;
        }
        if (parent->light == n) {
            parent->light = n->heavy;
            if (n->heavy) n->heavy->parent = parent;
            n->heavy = parent;
        } else {
            parent->heavy = n->light;
            if (n->light) n->light->parent = parent;
            n->light = parent;
        }
        parent->parent = n;
        n->parent = ancestor;
        derived()._update(parent);
        derived()._update(n);
    }

    // Recursively pushes all pending lazy operations from the splay tree root down to node `n`.
    void _push_until(node *n) {
        if (!_is_root(n)) _push_until(n->parent);
        derived()._push(n);
    }

    // Repeatedly rotates the splay tree until node `n` becomes the root.
    void _splay(node *n) {
        _push_until(n);
        while (!_is_root(n)) {
            node *parent = n->parent;
            if (!_is_root(parent)) {
                if ((parent->parent->light == parent) ^ (parent->light == n)) {
                    _rotate(n);
                } else {
                    _rotate(parent);
                }
            }
            _rotate(n);
        }
    }

    // Exposes the preferred path from node `n` to the root of its tree.
    // Moves `n` to the root of its splay tree.
    void _access(node *n) {
        node *last = nullptr;
        for (node *curr = n; curr; curr = curr->parent) {
            _splay(curr);
            curr->heavy = last;
            derived()._update(curr);
            last = curr;
        }
        _splay(n);
    }

    // Reorients the tree and moves node `n` to the root.
    void _make_root(node *n) {
        _access(n);
        n->lazy_reverse ^= true;
        derived()._push(n);
    }

    // Returns a pointer to the root of the tree containing node `n`.
    node* _get_root(node *n) {
        _access(n);
        while (n->light) {
            derived()._push(n);
            n = n->light;
        }
        _splay(n);
        return n;
    }

  public:
    // Constructs an empty link-cut tree.
    link_cut_tree_base() {}

    // Constructs a link-cut tree with the specified number of nodes.
    // Labels the nodes with indices `0, 1, ..., num_nodes - 1`.
    link_cut_tree_base(int num_nodes) {
        _nodes.reserve(num_nodes);
        for (int i = 0; i < num_nodes; i++) {
            _nodes.emplace_back(i);
        }
    }

    // Returns the number of nodes in the link-cut tree.
    int size() const {
        return static_cast<int>(_nodes.size());
    }

    // Adds a node to the link-cut tree and returns its index.
    int add_node() {
        int index = size();
        _nodes.emplace_back(index);
        return index;
    }

    // Assigns node `u` as a child of node `v` if `u` and `v` are disconnected.
    // Requires `u` and `v` to be valid indices.
    void link(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _make_root(node_u);
        if (_get_root(node_v) != node_u) node_u->parent = node_v;
    }

    // Removes the edge connecting nodes `u` and `v` if it exists.
    // Requires `u` and `v` to be valid indices.
    void cut(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _make_root(node_u);
        _access(node_v);
        if (node_v->light == node_u && !node_u->heavy) {
            node_v->light = nullptr;
            node_u->parent = nullptr;
            derived()._update(node_v);
            return;
        }
        _make_root(node_v);
        _access(node_u);
        if (node_u->light == node_v && !node_v->heavy) {
            node_u->light = nullptr;
            node_v->parent = nullptr;
            derived()._update(node_u);
        }
    }

    // Returns whether `u` and `v` are connected.
    // Requires `u` and `v` to be valid indices.
    int connected(int u, int v) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        return _get_root(&_nodes[u]) == _get_root(&_nodes[v]);
    }
};

// A link-cut tree with minimal functionalities.
struct link_cut_tree : link_cut_tree_base<link_cut_tree, link_cut_node> {};

struct extended_node {
    extended_node *parent = nullptr, *light = nullptr, *heavy = nullptr;
    bool lazy_reverse = false;
    int size = 1;
    int index;
    extended_node(int node_index) : index(node_index) {}
};

// A link-cut tree with extended functionalities.
struct extended_link_cut_tree : link_cut_tree_base<extended_link_cut_tree, extended_node> {
    friend class link_cut_tree_base<extended_link_cut_tree, extended_node>;
    using node = extended_node;

  protected:
    void _update(node *n) {
        if (!n) return;
        n->size = 1;
        if (n->light) n->size += n->light->size;
        if (n->heavy) n->size += n->heavy->size;
    }

    node* _get_nth(node *n, int index) {
        _push(n);
        int light_size = n->light ? n->light->size : 0;
        if (index < light_size) return _get_nth(n->light, index);
        if (index == light_size) return n;
        return _get_nth(n->heavy, index - light_size - 1);
    }

    void _collect_nodes(node *n, std::vector<int> &path) {
        if (!n) return;
        _push(n);
        _collect_nodes(n->light, path);
        path.push_back(n->index);
        _collect_nodes(n->heavy, path);
    }

  public:
    // Returns the number of nodes in the tree containing node `v`.
    // Requires `v` to be a valid index.
    int get_size(int v) {
        assert(0 <= v && v < size());
        return _get_root(&_nodes[v])->size;
    }

    // Returns the number of edges between nodes `u` and `v`.
    // If `u` and `v` are not connected, returns `-1` instead.
    // Requires `u` and `v` to be valid indices.
    int get_distance(int u, int v) {
        if (!connected(u, v)) return -1;
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _make_root(node_u);
        _access(node_v);
        return node_v->size - 1;
    }

    // Returns the index of the lowest common ancestor of nodes `u` and `v`.
    // If `u` and `v` are not connected, returns `-1` instead.
    // Requires `u` and `v` to be valid indices.
    int get_lca(int u, int v) {
        if (!connected(u, v)) return -1;
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _access(node_u);
        _access(node_v);
        _splay(node_u);
        if (_nodes[u].parent) return _nodes[u].parent->index;
        return u;
    }

    // Designates `root` as the new root of its tree and returns the lowest common ancestor of nodes `u` and `v`.
    // If either `u` or `v` is not in the same tree as `root`, returns `-1` instead.
    // Requires `u`, `v` and `root` to be valid indices.
    int get_lca(int u, int v, int root) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(0 <= root && root < size());
        node *node_u = &_nodes[u], *node_v = &_nodes[v], *node_r = &_nodes[root];
        _make_root(node_r);
        if (_get_root(node_u) != node_r || _get_root(node_v) != node_r) return -1;
        return get_lca(u, v);
    }

    // Returns the node at the specified index in the path from `u` to `v`.
    // Returns `u` if `index == 0`.
    // Returns `v` if `index == len` where `len` is the number of nodes in the path.
    // Returns `-1` instead if:
    // - `u` and `v` are not connected
    // - the path from `u` to `v` does not contain a node at the specified index
    //
    // Requires `u` and `v` to be valid indices.
    int get_nth_node(int u, int v, int index) {
        if (!connected(u, v)) return -1;
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _make_root(node_u);
        _access(node_v);
        if (index < 0 || index >= node_v->size) return -1;
        return _get_nth(node_v, index)->index;
    }

    // Returns the path from `u` to `v` as a vector of node indices.
    // If `u` and `v` are not connected, returns an empty vector.
    // Requires `u` and `v` to be valid indices.
    std::vector<int> get_path(int u, int v) {
        if (!connected(u, v)) return {};
        node *node_u = &_nodes[u], *node_v = &_nodes[v];
        _make_root(node_u);
        _access(node_v);
        std::vector<int> path;
        _collect_nodes(node_v, path);
        return path;
    }
};

}  // namespace kotone

#endif  // KOTONE_LINK_CUT_TREE
