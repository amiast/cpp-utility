#ifndef KOTONE_MERGESORTTREE_HPP
#define KOTONE_MERGESORTTREE_HPP

#include <vector>
#include <algorithm>
#include <iterator>

namespace kotone {

template <typename T>
class mergesorttree {
  private:
    std::vector<std::vector<T>> tree;
    int N;

    void build(std::vector<T> &vec, int node, int left, int right) {
        if (left + 1 == right) {
            tree[node] = {vec[left]};
            return;
        }

        int mid = (left + right) / 2;
        build(vec, node * 2 + 1, left, mid);
        build(vec, node * 2 + 2, mid, right);
        std::merge(
            tree[node * 2 + 1].begin(), tree[node * 2 + 1].end(),
            tree[node * 2 + 2].begin(), tree[node * 2 + 2].end(),
            std::back_inserter(tree[node])
        );
    }

    int query(int node, int node_left, int node_right, int l, int r, T x) {
        if (node_right <= l || node_left >= r) return 0;
        if (l <= node_left && node_right <= r) {
            return std::upper_bound(tree[node].begin(), tree[node].end(), x) - tree[node].begin();
        }
        int mid = (node_left + node_right) / 2;
        return query(node * 2 + 1, node_left, mid, l, r, x) + query(node * 2 + 2, mid, node_right, l, r, x);
    }

  public:
    mergesorttree(std::vector<T> &vec) {
        N = vec.size();
        tree.resize(N * 4);
        build(vec, 0, 0, N);
    }

    int count(int l, int r, T x) {
        return query(0, 0, N, l, r, x);
    }
};

}  // namespace kotone

#endif // KOTONE_MERGESORTTREE_HPP
