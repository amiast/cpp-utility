#ifndef KOTONE_TREAP_HPP
#define KOTONE_TREAP_HPP 1

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <kotone/random>
#include <kotone/memory_pool>

namespace kotone {

// A self-contained memory manager for implicit treaps.
// Requires the following functions:
// - `S op(S val_l, S val_r)`
// - `S e()`
//
// Reference: https://nyaannyaan.github.io/library/rbst/treap.hpp
template <typename S, S (*op)(S, S), S (*e)()> struct treap_manager {
    // A node for treap-related operations.
    struct treap {
        friend struct treap_manager;

      private:
        const uint64_t _PRIORITY = randint();
        treap *_left = nullptr, *_right = nullptr;
        S _val = e(), _prod = e(), _prod_rev = e();
        int _size = 1;
        bool _rev = false;

      public:
        // Constructs a treap node with value initialized to `e()`.
        treap() {}

        // Constructs a treap node with the specified value.
        treap(S val) : _val(val), _prod(val), _prod_rev(val) {}
    };

  private:
    memory_pool<treap> _pool;

    void _push(treap *node) const {
        if (!node) return;
        if (node->_rev) {
            reverse(node->_left);
            reverse(node->_right);
            node->_rev = false;
        }
    }

    void _update(treap *node) const {
        if (!node) return;
        _push(node);
        node->_size = 1;
        node->_prod = node->_prod_rev = node->_val;
        if (node->_left) {
            node->_size += node->_left->_size;
            node->_prod = op(node->_left->_prod, node->_prod);
            node->_prod_rev = op(node->_prod_rev, node->_left->_prod_rev);
        }
        if (node->_right) {
            node->_size += node->_right->_size;
            node->_prod = op(node->_prod, node->_right->_prod);
            node->_prod_rev = op(node->_right->_prod_rev, node->_prod_rev);
        }
    }

    void _init_treap(treap *root) const {
        if (!root) return;
        _init_treap(root->_left);
        _init_treap(root->_right);
        _update(root);
    }

    void _to_vector(treap *root, std::vector<S> &vec) const {
        if (!root) return;
        _push(root);
        _to_vector(root->_left, vec);
        vec.push_back(root->_val);
        _to_vector(root->_right, vec);
    }

    template <typename G> int _max_right(treap *root, const G &g, S &acc) const {
        if (!root) return 0;
        _push(root);
        S new_acc = op(acc, root->_prod);
        if (g(new_acc)) {
            acc = new_acc;
            return size(root);
        }
        int left_size = size(root->_left);
        int result = _max_right(root->_left, g, acc);
        if (result < left_size) return result;
        new_acc = op(acc, root->_val);
        if (!g(new_acc)) return left_size;
        acc = new_acc;
        return _max_right(root->_right, g, acc) + left_size + 1;
    }

    template <typename G> int _min_left(treap *root, const G &g, S &acc) const {
        if (!root) return 0;
        _push(root);
        S new_acc = op(root->_prod, acc);
        if (g(new_acc)) {
            acc = new_acc;
            return 0;
        }
        int left_size = size(root->_left);
        int result = _min_left(root->_right, g, acc);
        if (result > 0) return result + left_size + 1;
        new_acc = op(root->_val, acc);
        if (!g(new_acc)) return left_size + 1;
        acc = new_acc;
        return _min_left(root->_left, g, acc);
    }

  public:
    // Constructs and returns a pointer to a treap node with value initialized to `e()`.
    treap* allocate_node() {
        return _pool.allocate();
    }

    // Constructs and returns a pointer to a treap node with the specified value.
    treap* allocate_node(S val) {
        return _pool.allocate(val);
    }

    // Converts the vector to a treap and returns a pointer to the root of the treap.
    // If `vec` is empty, returns `nullptr`.
    treap* allocate_treap(const std::vector<S> &vec) {
        if (vec.empty()) return nullptr;
        std::vector<treap*> stack;
        treap *root = nullptr;
        for (S val : vec) {
            treap *node = _pool.allocate(val);
            treap *last = nullptr;
            while (stack.size() && stack.back()->_PRIORITY < node->_PRIORITY) {
                last = stack.back();
                stack.pop_back();
            }
            node->_left = last;
            if (stack.size()) stack.back()->_right = node;
            else root = node;
            stack.push_back(node);
        }
        _init_treap(root);
        return root;
    }

    // Frees all memory allocated to nodes in the treap and invalidates their pointers.
    void deallocate_treap(treap *root) {
        if (!root) return;
        deallocate_treap(root->_left);
        deallocate_treap(root->_right);
        _pool.deallocate(root);
    }

    // Returns the size of the treap.
    int size(treap *root) const {
        if (root) return root->_size;
        return 0;
    }

    // Returns a vector containing the values of the treap.
    std::vector<S> to_vector(treap *root) const {
        std::vector<S> result;
        result.reserve(size(root));
        _to_vector(root, result);
        return result;
    }

    // Merges `root_l` and `root_r`, then returns a pointer to the root of the new treap.
    // This method invalidates `root_l` and `root_r`.
    // Requires `root_l` and `root_r` to be disconnected.
    treap* merge(treap *root_l, treap *root_r) const {
        assert(!root_l || !root_r || root_l != root_r);
        _push(root_l);
        _push(root_r);
        treap *result;
        if (!root_l || !root_r) {
            result = root_l ? root_l : root_r;
        } else if (root_l->_PRIORITY >= root_r->_PRIORITY) {
            root_l->_right = merge(root_l->_right, root_r);
            result = root_l;
        } else {
            root_r->_left = merge(root_l, root_r->_left);
            result = root_r;
        }
        _update(result);
        return result;
    }

    // Splits the treap at the specified index and returns a pair of pointer.
    // The first pointer is the root of the interval `[0, index)`.
    // The second pointer is the root of the interval `[index, size(root))`.
    // This method invalidates `root`.
    // Requires `0 <= index <= size(root)`.
    std::pair<treap*, treap*> split(treap *root, int index) const {
        assert(0 <= index && index <= size(root));
        if (!root) return {nullptr, nullptr};
        _push(root);
        int left_size = size(root->_left);
        if (index <= left_size) {
            auto [l, r] = split(root->_left, index);
            root->_left = r;
            _update(root);
            return {l, root};
        } else {
            auto [l, r] = split(root->_right, index - left_size - 1);
            root->_right = l;
            _update(root);
            return {root, r};
        }
    }

    // Returns the product of the treap.
    // If the treap is empty, returns `e()`.
    S get_prod(treap *root) const {
        _push(root);
        if (root) return root->_prod;
        return e();
    }

    // Returns the product of the interval `[l, r)` in the treap.
    // Requires `0 <= l <= r <= size(root)`.
    S get_prod(treap *&root, int l, int r) const {
        assert(0 <= l && l <= r && r <= size(root));
        if (l == r) return e();
        auto [nl, nml] = split(root, l);
        auto [nmr, nr] = split(nml, r - l);
        S result = nmr->_prod;
        root = merge(merge(nl, nmr), nr);
        return result;
    }

    // Returns the value of the node at the specified index in the treap.
    // Requires `0 <= index < size(root)`.
    S get_val(treap *root, int index) const {
        assert(0 <= index && index < size(root));
        treap *curr = root;
        while (true) {
            _push(curr);
            int left_size = size(curr->_left);
            if (index == left_size) return curr->_val;
            if (index < left_size) curr = curr->_left;
            else curr = curr->_right, index -= left_size + 1;
        }
    }

    // Reassigns the value of the node at the specified index in the treap.
    // Requires `0 <= index < size(node)`.
    void set_val(treap *&root, int index, S val) const {
        assert(0 <= index && index < size(root));
        auto [nl, nml] = split(root, index);
        auto [nmr, nr] = split(nml, 1);
        nmr->_val = val;
        root = merge(merge(nl, nmr), nr);
    }

    // Inserts `new_treap` at the specified index in the treap.
    // Requires `root` and `new_treap` to be disconnected.
    // Requires `0 <= index <= size(root)`.
    void insert(treap *&root, treap *new_treap, int index) const {
        assert(!root || !new_treap || root != new_treap);
        assert(0 <= index && index <= size(root));
        if (!new_treap) return;
        auto [nl, nr] = split(root, index);
        root = merge(merge(nl, new_treap), nr);
    }

    // Removes the specified interval `[l, r)` from the treap,
    // then returns a pointer to the root of the treap in the removed interval.
    // Requires `0 <= l <= r <= size(root)`.
    treap* remove(treap *&root, int l, int r) const {
        assert(0 <= l && l <= r && r <= size(root));
        if (l == r) return nullptr;
        auto [nl, nml] = split(root, l);
        auto [nmr, nr] = split(nml, r - l);
        root = merge(nl, nr);
        return nmr;
    }

    // Removes the node at the specified index from the treap and returns a pointer to the removed node.
    // Requires `0 <= index < size(root)`.
    treap* remove(treap *&root, int index) const {
        assert(0 <= index && index < size(root));
        return remove(root, index, index + 1);
    }

    // Reverses the entire treap.
    void reverse(treap *root) const {
        if (!root) return;
        std::swap(root->_left, root->_right);
        std::swap(root->_prod, root->_prod_rev);
        root->_rev ^= true;
    }

    // Reverses the specified interval `[l, r)` in the treap.
    // Requires `0 <= l <= r <= size(root)`.
    void reverse(treap *&root, int l, int r) const {
        assert(0 <= l && l <= r && r <= size(root));
        if (l == r) return;
        auto [nl, nml] = split(root, l);
        auto [nmr, nr] = split(nml, r - l);
        reverse(nmr);
        root = merge(merge(nl, nmr), nr);
    }

    // Returns the maximum `r` such that `g(get_prod(root, l, r)) == true`.
    // Requires `0 <= l <= size(root)`.
    // Requires `bool g(S val)` to be a monotonic predicate.
    // Requires `g(e()) == true`.
    template <typename G> int max_right(treap *&root, int l, G g) const {
        assert(0 <= l && l <= size(root));
        assert(g(e()));
        if (l == size(root)) return l;
        auto [nl, nr] = split(root, l);
        S acc = e();
        int result = _max_right(nr, g, acc) + l;
        root = merge(nl, nr);
        return result;
    }

    // Returns the minimum `l` such that `g(get_prod(root, l, r)) == true`.
    // Requires `0 <= r <= size(root)`.
    // Requires `bool g(S val)` to be a monotonic predicate.
    // Requires `g(e()) == true`.
    template <typename G> int min_left(treap *&root, int r, G g) const {
        assert(0 <= r && r <= size(root));
        assert(g(e()));
        if (r == 0) return r;
        auto [nl, nr] = split(root, r);
        S acc = e();
        int result = _min_left(nl, g, acc);
        root = merge(nl, nr);
        return result;
    }
};

}  // namespace kotone

#endif  // KOTONE_TREAP_HPP
