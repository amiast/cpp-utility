#ifndef KOTONE_RBST_REVERSIBLE_HPP
#define KOTONE_RBST_REVERSIBLE_HPP 1

#include <vector>
#include <random>
#include <cassert>

namespace kotone {

// Maintains a forest of reversible randomized binary search tree (reversible RBST) with lazy propagation.
// Stores a dummy node at index `-1` to represent an empty tree with value `e()`.
// If `is_persistent == false`, operations that expect to copy a node will instead receive the same node.
// The dummy node cannot be copied.
template <
    typename S,
    S (*op)(S, S),
    S (*e)(),
    typename F,
    S (*mapping)(F, S),
    F (*composition)(F, F),
    F (*id)(),
    bool is_persistent
> struct reversible_randomized_binary_search_tree {
  private:
    struct node {
        S val = e(), acc = e(), acc_rev = e();
        F lazy = id();
        int left = 0, right = 0, size = 0;
        bool rev = false;
    };

    std::vector<node> _tree;

    int _copy(int i) {
        if constexpr (!is_persistent) return i;
        _tree.push_back(_tree[i]);
        return _tree.size() - 1;
    }

    template <bool copying> int _apply(int i, F app) {
        if constexpr (is_persistent && copying) i = _copy(i);
        _tree[i].val = mapping(app, _tree[i].val);
        _tree[i].acc = mapping(app, _tree[i].acc);
        _tree[i].acc_rev = mapping(app, _tree[i].acc_rev);
        _tree[i].lazy = composition(app, _tree[i].lazy);
        return i;
    }

    void _reverse(int i) {
        if (!i) return;
        std::swap(_tree[i].acc, _tree[i].acc_rev);
        std::swap(_tree[i].left, _tree[i].right);
        _tree[i].rev ^= true;
    }

    void _push(int i) {
        F app = _tree[i].lazy;
        _tree[i].lazy = id();
        if (_tree[i].left) _tree[i].left = _apply<true>(_tree[i].left, app);
        if (_tree[i].right) _tree[i].right = _apply<true>(_tree[i].right, app);
        if (_tree[i].rev) {
            _reverse(_tree[i].left);
            _reverse(_tree[i].right);
            _tree[i].rev = false;
        }
    }

    void _update(int i) {
        _tree[i].acc = _tree[i].acc_rev = _tree[i].val;
        _tree[i].size = _tree[_tree[i].left].size + _tree[_tree[i].right].size + 1;
        if (int l = _tree[i].left; l) {
            _tree[i].acc = op(_tree[l].acc, _tree[i].acc);
            _tree[i].acc_rev = op(_tree[i].acc_rev, _tree[l].acc_rev);
        }
        if (int r = _tree[i].right; r) {
            _tree[i].acc = op(_tree[i].acc, _tree[r].acc);
            _tree[i].acc_rev = op(_tree[r].acc_rev, _tree[i].acc_rev);
        }
    }

    std::pair<int, int> _split(int i, int m) {
        if (!i) return {0, 0};
        i = _copy(i);
        _push(i);
        int s = _tree[_tree[i].left].size;
        if (m <= s) {
            auto [l, r] = _split(_tree[i].left, m);
            _tree[i].left = r;
            _update(i);
            return {l, i};
        }
        auto [l, r] = _split(_tree[i].right, m - s - 1);
        _tree[i].right = l;
        _update(i);
        return {i, r};
    }

    int _merge(int l, int r) {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        _push(l);
        _push(r);
        if (!l) return _copy(r);
        if (!r) return _copy(l);
        if (int(gen() % (_tree[l].size + _tree[r].size)) < _tree[l].size) {
            l = _copy(l);
            _tree[l].right = _merge(_tree[l].right, r);
            _update(l);
            return l;
        }
        r = _copy(r);
        _tree[r].left = _merge(l, _tree[r].left);
        _update(r);
        return r;
    }

    void _to_vector(int i, std::vector<S> &vec) {
        if (!i) return;
        _push(i);
        _to_vector(_tree[i].left, vec);
        vec.push_back(_tree[i].val);
        _to_vector(_tree[i].right, vec);
    }

  public:
    // Constructs an empty RBST.
    reversible_randomized_binary_search_tree() : _tree(1) {}

    // Constructs a RBST from the given `vec` and sets `root` to the index of its root.
    reversible_randomized_binary_search_tree(int &root, const std::vector<S> &vec) : _tree(1) {
        root = make_tree(vec);
    }

    // Constructs a RBST for a sequence of `e()` with the specified size,
    // then sets `root` to the index of its root.
    // Requires `size > 0`.
    reversible_randomized_binary_search_tree(int &root, int size) : _tree(1) {
        assert(size > 0);
        root = make_tree(size);
    }

    // Returns the number of nodes constructed so far.
    // Does not include the empty tree at index `-1`.
    int size() const {
        return _tree.size() - 1;
    }

    // Constructs a RBST node and returns its index.
    int make_node(S val) {
        int i = size() + 1;
        _tree.emplace_back();
        _tree[i].val = std::move(val);
        _update(i);
        return i - 1;
    }

    // Constructs a RBST for the specified `vec` and returns the index of its root.
    int make_tree(const std::vector<S> &vec) {
        int n = vec.size();
        int s = size() + 1;
        _tree.resize(s + n);
        auto build = [&](auto &build, int l, int r) {
            if (l == r) return 0;
            int m = (l + r) / 2;
            _tree[s + m].val = vec[m];
            _tree[s + m].left = build(build, l, m);
            _tree[s + m].right = build(build, m + 1, r);
            _update(s + m);
            return s + m;
        };
        return build(build, 0, n) - 1;
    }

    // Constructs a RBST for a sequence of `e()` with the specified length and returns the index of its root.
    // Requires `length > 0`.
    int make_tree(int length) {
        assert(length > 0);
        int s = size() + 1;
        _tree.resize(s + length);
        auto build = [&](auto &build, int l, int r) {
            if (l == r) return 0;
            int m = (l + r) / 2;
            _tree[s + m].left = build(build, l, m);
            _tree[s + m].right = build(build, m + 1, r);
            _update(s + m);
            return s + m;
        };
        return build(build, 0, length) - 1;
    }

    // Returns the number of nodes in the RBST rooted at `i`.
    // If `i == -1`, returns `0`.
    // Requires `0 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    int size(int i) const {
        assert(-1 <= i && i < size());
        return _tree[i + 1].size;
    }

    // Copies the RBST rooted at `i` then returns the index of its root.
    // If `i == -1`, returns `-1`.
    // Requires `0 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    int copy(int i) {
        assert(-1 <= i && i < size());
        return _copy(i + 1) - 1;
    }

    // Returns the value of node `i`.
    // If `i == -1`, returns `e()`.
    // Requires `-1 <= i < size()`.
    S get(int i) const {
        assert(-1 <= i && i < size());
        return _tree[i + 1].val;
    }

    // Copies the RBST rooted at `i` and updates the value of its root,
    // then returns the index of its root.
    // Requires `0 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    int set(int i, S val) {
        assert(0 <= i && i < size());
        i++;
        i = _copy(i);
        _tree[i].val = val;
        _update(i);
        return i - 1;
    }

    // Returns the product of the RBST rooted at `i`.
    // If `i == -1`, returns `e()`.
    // Requires `-1 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    S prod(int i) const {
        assert(-1 <= i && i < size());
        return _tree[i + 1].acc;
    }

    // Copies the RBST rooted at `i` and applies update,
    // then returns the index of its root.
    // If `i == -1`, returns `-1` without modification.
    // Requires `-1 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    int apply(int i, F app) {
        assert(-1 <= i && i < size());
        if (i == -1) return -1;
        i++;
        i = _copy(i);
        _apply<false>(i, app);
        return i - 1;
    }

    // Copies and reverses the RBST rooted at `i` then returns the index of its root.
    // If `i == -1`, returns `-1` without modification.
    // Requires `-1 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    int reverse(int i) {
        assert(-1 <= i && i < size());
        i++;
        i = _copy(i);
        _reverse(i);
        return i - 1;
    }

    // Copies the RBST rooted at `i` and splits it into two intervals `[0, m)` and `[m, size(i))`,
    // then returns a pair of indices of the roots.
    // If the relevant subtree is empty, returns `-1` in place of its index.
    // If `i == -1`, returns `{-1, -1}` without copying.
    // Requires `-1 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    // Requires `0 <= m <= size(i)`.
    std::pair<int, int> split(int i, int m) {
        assert(-1 <= i && i < size());
        assert(0 <= m && m <= size(i));
        i++;
        auto [l, r] = _split(i, m);
        return {l - 1, r - 1};
    }

    // Copies the RBSTs rooted at `l, r` and merges them into a single tree,
    // then returns the index of its root.
    // If `l == r == -1`, returns `-1` without copying.
    // Requires `-1 <= l, r < size()`.
    // If `l != -1 && r != -1`, requires `l, r` to be the indices of the roots of distinct RBSTs.
    int merge(int l, int r) {
        assert(-1 <= l && l < size());
        assert(-1 <= r && r < size());
        assert(l == -1 || l != r);
        l++, r++;
        return _merge(l, r) - 1;
    }

    // Returns the values of the RBST rooted at index `i`.
    // If `i == -1`, returns an empty vector.
    // Requires `-1 <= i < size()`.
    // Requires `i` to be the index of the root of a RBST.
    std::vector<S> to_vector(int i) {
        assert(-1 <= i && i < size());
        std::vector<S> vec;
        vec.reserve(size(i));
        _to_vector(i + 1, vec);
        return vec;
    }

    // Resets the RBSTs.
    void clear() {
        _tree.resize(1);
    }
};

}  // namespace kotone

#endif  // KOTONE_RBST_REVERSIBLE_HPP
