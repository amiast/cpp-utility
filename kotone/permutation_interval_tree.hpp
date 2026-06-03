#ifndef KOTONE_PERMUTATION_INTERVAL_TREE_HPP
#define KOTONE_PERMUTATION_INTERVAL_TREE_HPP 1

#include <vector>
#include <cassert>
#include <atcoder/lazysegtree>

namespace kotone {

// A node representing a connected interval in a permutation interval tree.
// A half-open interval `[l, r)` with minimum `mn` and past-the-end maximum `mx` is connected if `r-l == mx-mn`.
// Includes the following attributes:
// - `start`: the index of the start of the interval.
// - `stop` the past-the-end index of the end of the interval.
// - `min`: the minimum value of the interval.
// - `max`: the past-the-end maximum value of the interval.
// - `parent`: the index of the parent node if it exists, otherwise `-1`.
// - `is_linear`: a `bool` indicating whether every concatenation of neighboring child intervals is connected.
// - `children`: a `vector` of indices of the interval's children in the order of appearance.
//
// Reference: https://judge.yosupo.jp/problem/common_interval_decomposition_tree
struct permutation_interval_node {
    int start, stop, min, max, parent;
    bool is_linear;
    std::vector<int> children;
};

// Returns a `pair` representing the permutation interval tree for the specified permutation.
// Includes the following attributes:
// - `first`: a `vector` of `permutation_interval_node` in the tree. If `perm` is empty, `first` is also empty.
// - `second`: the index of the root of the tree. If `perm` is empty, `second == -1`.
//
// Requires `perm` to be a permutation of `(0, ..., perm.size() - 1)`.
//
// Reference: https://judge.yosupo.jp/problem/common_interval_decomposition_tree
std::pair<std::vector<permutation_interval_node>, int> construct_permutation_interval_tree(const std::vector<int> &perm) {
    int n = perm.size();
    if (n == 0) return {{}, -1};
    {
        std::vector<int> pos(n, -1);
        for (int i = 0; i < n; i++) {
            assert(0 <= perm[i] && perm[i] < n);
            assert(pos[perm[i]] == -1);
            pos[perm[i]] = i;
        }
    }
    atcoder::lazy_segtree<
        int,
        [](int a, int b){ return std::max(a, b); },
        [](){ return -1 << 30; },
        int,
        [](int f, int x){ return f + x; },
        [](int f, int g){ return f + g; },
        [](){ return 0; }
    > seg(n);
    std::vector<permutation_interval_node> result;
    std::vector<int> node_stack, min_stack{-1}, max_stack{-1};
    for (int r = 0; r < n; r++) {
        auto update = [&](std::vector<int> &st, int sign) {
            int m = r;
            while (st.back() != -1 && (perm[st.back()] - perm[r]) * sign > 0) {
                st.pop_back();
                int l = st.back() + 1;
                seg.apply(l, m, (perm[r] - perm[m - 1]) * sign);
                m = l;
            }
            st.push_back(r);
        };
        update(min_stack, 1);
        update(max_stack, -1);
        seg.apply(0, r, 1);
        int curr = result.size();
        result.emplace_back(r, r + 1, perm[r], perm[r] + 1, -1, true);
        while (true) {
            if (
                node_stack.size()
                && result[node_stack.back()].children.size()
                && result[node_stack.back()].is_linear
                && (
                    result[curr].min == result[result[node_stack.back()].children.back()].max
                    || result[curr].max == result[result[node_stack.back()].children.back()].min
                )
            ) {
                permutation_interval_node &par = result[node_stack.back()];
                par.children.push_back(curr);
                par.min = std::min(par.min, result[curr].min);
                par.max = std::max(par.max, result[curr].max);
                par.stop = r + 1;
                result[curr].parent = node_stack.back();
                curr = node_stack.back();
                node_stack.pop_back();
                continue;
            }
            if (
                node_stack.size()
                && (
                    result[curr].min == result[node_stack.back()].max
                    || result[curr].max == result[node_stack.back()].min
                )
            ) {
                int next = result.size();
                result.emplace_back(
                    result[node_stack.back()].start,
                    result[curr].stop,
                    std::min(result[node_stack.back()].min, result[curr].min),
                    std::max(result[node_stack.back()].max, result[curr].max),
                    -1,
                    true,
                    std::vector{node_stack.back(), curr}
                );
                result[node_stack.back()].parent = next;
                node_stack.pop_back();
                result[curr].parent = next;
                curr = next;
                continue;
            }
            node_stack.push_back(curr);
            if (node_stack.size() == 1u) break;
            if (seg.prod(0, result[curr].start) != 0) break;
            int next = result.size();
            result.emplace_back(
                result[curr].start,
                result[curr].stop,
                result[curr].min,
                result[curr].max,
                -1,
                false,
                std::vector{curr}
            );
            result[curr].parent = next;
            curr = next;
            node_stack.pop_back();
            permutation_interval_node &par = result[curr];
            do {
                permutation_interval_node &ch = result[node_stack.back()];
                par.start = std::min(par.start, ch.start);
                par.stop = std::max(par.stop, ch.stop);
                par.min = std::min(par.min, ch.min);
                par.max = std::max(par.max, ch.max);
                par.children.push_back(node_stack.back());
                ch.parent = curr;
                node_stack.pop_back();
            } while (par.stop - par.start != par.max - par.min);
            std::reverse(par.children.begin(), par.children.end());
        }
        seg.set(r, 0);
    }
    return {result, node_stack.back()};
}

}  // namespace kotone

#endif  // KOTONE_PERMUTATION_INTERVAL_TREE_HPP
