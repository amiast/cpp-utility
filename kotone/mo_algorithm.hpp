#ifndef KOTONE_MO_ALGORITHM_HPP
#define KOTONE_MO_ALGORITHM_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>

namespace kotone {

// A data structure that maintains range queries on a sequence of elements.
// Also supports two-dimensional queries in general.
// Reference: https://take44444.github.io/Algorithm-Book/range/mo/main.html
struct mo_alg {
  private:
    int _num_queries = 0;
    std::vector<int> _l, _r;

  public:
    // Inserts a range query `[l, r)` and returns the index of the given query.
    // Requires `l` and `r` to be non-negative.
    int insert_query(int l, int r) {
        assert(l >= 0);
        assert(r >= 0);
        _l.emplace_back(l);
        _r.emplace_back(r);
        return _num_queries++;
    }

    // Evaluates range queries using the provided functions.
    // Specialized for operations that depend on both dimensions.
    // - `order(l, r)` returns the index of `[l, r)` for sorting queries.
    // - `add_l(l, r)` modifies `[l + 1, r)` into `[l, r)`.
    // - `add_r(l, r)` modifies `[l, r)` into `[l, r + 1)`.
    // - `del_l(l, r)` modifies `[l, r)` into `[l + 1, r)`.
    // - `del_r(l, r)` modifies `[l, r + 1)` into `[l, r)`.
    // - `solve(i)` evaluates the `i`-th query.
    template <typename order_, typename add_l_, typename add_r_, typename del_l_, typename del_r_, typename solve_>
    void eval_queries(order_ &&order, add_l_ &&add_l, add_r_ && add_r, del_l_ &&del_l, del_r_ &&del_r, solve_ &&solve) {
        std::vector<int> indices(_num_queries);
        std::vector<int64_t> ordering(_num_queries);
        for (int i = 0; i < _num_queries; i++) {
            indices[i] = i;
            ordering[i] = order(_l[i], _r[i]);
        }
        std::sort(indices.begin(), indices.end(), [&ordering](int i, int j){ return ordering[i] < ordering[j]; });
        int nl = 0, nr = 0;
        for (int i : indices) {
            while (nl > _l[i]) add_l(--nl, nr);
            while (nr < _r[i]) add_r(nl, nr++);
            while (nl < _l[i]) del_l(nl++, nr);
            while (nr > _r[i]) del_r(nl, --nr);
            solve(i);
        }
    }

    // Evaluates range queries using the provided functions.
    // Specialized for operations that depend only on the added/deleted dimension.
    // - `order(l, r)` returns the index of `[l, r)` for sorting queries.
    // - For arbitrary `y`, `add(x)` modifies:
    // ** `[x + 1, y)` into `[x, y)`, and
    // ** `[y, x)` into `[y, x + 1)`.
    // - For arbitrary `y`, `del(x)` modifies:
    // ** `[x, y)` into `[x + 1, y)`, and
    // ** `[y, x + 1)` into `[y, x)`.
    // - `solve(i)` evaluates the `i`-th query.
    template <typename order_, typename add_, typename del_, typename solve_>
    void eval_queries(order_ &&order, add_ &&add, del_ &&del, solve_ &&solve) {
        auto add_l = [add](int l, int) { add(l); };
        auto add_r = [add](int, int r) { add(r); };
        auto del_l = [del](int l, int) { del(l); };
        auto del_r = [del](int, int r) { del(r); };
        eval_queries(order, add_l, add_r, del_l, del_r, solve);
    }
};

// Returns the index of the given coordinates `(x, y)` on a Hilbert curve.
// Requires `x` and `y` to be non-negative integers less than `1 << max_bit_width`.
// Requires `bit_width` to be a positive integer at most `30`.
// Reference: https://take44444.github.io/Algorithm-Book/range/mo/main.html
int64_t hilbert_index(int x, int y, int max_bit_width = 30) {
    assert(0 < max_bit_width && max_bit_width <= 30);
    assert(0 <= x && x < 1 << max_bit_width);
    assert(0 <= y && y < 1 << max_bit_width);
    int64_t rx, ry, index = 0;
    int N = 1 << max_bit_width;
    for (int64_t s = N >> 1; s; s >>= 1) {
        rx = (x & s) > 0, ry = (y & s) > 0;
        index += s * s * ((rx * 3) ^ ry);
        if (ry) continue;
        if (rx) {
            x = N - 1 - x;
            y = N - 1 - y;
        }
        std::swap(x, y);
    }
    return index;
}

}  // namespace kotone

#endif  // KOTONE_MO_ALGORITHM_HPP
