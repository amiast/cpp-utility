#ifndef KOTONE_MERGESORT_TREE_HPP
#define KOTONE_MERGESORT_TREE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace kotone {

template <typename T>
class mergesort_tree {
  private:
    std::vector<std::vector<T>> _tree;
    int _len;

    void _build(std::vector<T> &vec, int node, int left, int right) {
        if (left + 1 == right) {
            _tree[node] = {vec[left]};
            return;
        }

        int mid = (left + right) / 2;
        _build(vec, node * 2 + 1, left, mid);
        _build(vec, node * 2 + 2, mid, right);
        std::merge(
            _tree[node * 2 + 1].begin(), _tree[node * 2 + 1].end(),
            _tree[node * 2 + 2].begin(), _tree[node * 2 + 2].end(),
            std::back_inserter(_tree[node])
        );
    }

    int _query(int node, int node_left, int node_right, int l, int r, T x) {
        if (node_right <= l || node_left >= r) return 0;
        if (l <= node_left && node_right <= r) {
            return std::distance(
                _tree[node].begin(),
                std::upper_bound(_tree[node].begin(), _tree[node].end(), x)
            );
        }
        int mid = (node_left + node_right) / 2;
        return _query(node * 2 + 1, node_left, mid, l, r, x) + _query(node * 2 + 2, mid, node_right, l, r, x);
    }

  public:
    mergesort_tree() {}

    // Instantiates a mergesort tree for the given `vec`.
    mergesort_tree(std::vector<T> &vec) : _len(static_cast<int>(vec.size())), _tree(vec.size() * 4) {
        assert(vec.size() <= 100000000u);
        _build(vec, 0, 0, _len);
    }

    // Returns the number of elements in `[l, r)` that are at most `x`.
    // Requires `[l, r)` to be a valid range.
    int count(int l, int r, T x) {
        assert(0 <= l && l <= r && r <= _len);
        return _query(0, 0, _len, l, r, x);
    }
};

}  // namespace kotone

#endif // KOTONE_MERGESORTTREE_HPP
