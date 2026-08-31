#ifndef KOTONE_MO_ALGORITHM_HPP
#define KOTONE_MO_ALGORITHM_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>

namespace kotone {

// Processes offline range queries (and 2D queries in general).
// Requires the following functions:
// - `order(l, r)` returns the index of `[l, r)` for sorting queries.
// - `incr_l(l, r)` modifies `[l, r)` into `[l + 1, r)`.
// - `decr_l(l, r)` modifies `[l + 1, r)` into `[l, r)`.
// - `incr_r(l, r)` modifies `[l, r)` into `[l, r + 1)`.
// - `decr_r(l, r)` modifies `[l, r + 1)` into `[l, r)`.
// - `solve(i)` evaluates the `i`-th query.
//
// Requires `l, r >= 0` for all queries `[l, r)`.
// Reference: https://take44444.github.io/Algorithm-Book/range/mo/main.html
template <typename order_, typename incr_l_, typename decr_l_, typename incr_r_, typename decr_r_, typename solve_>
void eval_range_queries(
    const std::vector<std::pair<int, int>> &queries,
    order_ &&order,
    incr_l_ &&incr_l,
    decr_l_ &&decr_l,
    incr_r_ && incr_r,
    decr_r_ &&decr_r,
    solve_ &&solve
) {
    for (auto [l, r] : queries) {
        assert(l >= 0);
        assert(r >= 0);
    }
    int n = queries.size();
    std::vector<int> indices(n);
    std::vector<int64_t> ordering(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
        ordering[i] = order(queries[i].first, queries[i].second);
    }
    std::sort(indices.begin(), indices.end(), [&ordering](int i, int j){ return ordering[i] < ordering[j]; });
    int l = 0, r = 0;
    for (int i : indices) {
        while (l > queries[i].first) decr_l(--l, r);
        while (r < queries[i].second) incr_r(l, r++);
        while (l < queries[i].first) incr_l(l++, r);
        while (r > queries[i].second) decr_r(l, --r);
        solve(i);
    }
}

// Processes offline range queries with specialized add/del operations.
// Requires the following functions:
// - `order(l, r)` returns the index of `[l, r)` for sorting queries.
// - For arbitrary `y`, `add(x)` modifies:
// ** `[x + 1, y)` into `[x, y)`, and
// ** `[y, x)` into `[y, x + 1)`.
// - For arbitrary `y`, `del(x)` modifies:
// ** `[x, y)` into `[x + 1, y)`, and
// ** `[y, x + 1)` into `[y, x)`.
// - `solve(i)` evaluates the `i`-th query.
//
// Requires `l, r >= 0` for all queries `[l, r)`.
// Reference: https://take44444.github.io/Algorithm-Book/range/mo/main.html
template <typename order_, typename add_, typename del_, typename solve_>
void eval_range_queries_add_del(
    const std::vector<std::pair<int, int>> &queries,
    order_ &&order,
    add_ &&add,
    del_ &&del,
    solve_ &&solve
) {
    auto decr_l = [add](int l, int) { add(l); };
    auto incr_r = [add](int, int r) { add(r); };
    auto incr_l = [del](int l, int) { del(l); };
    auto decr_r = [del](int, int r) { del(r); };
    eval_range_queries(queries, order, incr_l, decr_l, incr_r, decr_r, solve);
}

// Returns the index of the given coordinates `(x, y)` on a Hilbert curve.
// Requires `0 <= bit_width <= 30`.
// Requires `0 <= x < 1 << max_bit_width`.
// Requires `0 <= y < 1 << max_bit_width`.
// Reference: https://take44444.github.io/Algorithm-Book/range/mo/main.html
template <int max_bit_width = 30> int64_t hilbert_index(int x, int y) {
    static_assert(0 < max_bit_width && max_bit_width <= 30);
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

// Returns the index of the given coordinates `(x, y)` on a zigzagging path.
// Requires `0 < block_width <= width`.
// Requires `0 <= x < width`.
// Requires `0 <= y < width`.
template <int width, int block_width> int64_t zigzag_index(int x, int y) {
    static_assert(0 < block_width && block_width <= width);
    assert(0 <= x && x < width);
    assert(0 <= y && y < width);
    int64_t p = x / block_width;
    int64_t q = p % 2 == 0 ? y : width - 1 - y;
    return p * width + q;
}

}  // namespace kotone

#endif  // KOTONE_MO_ALGORITHM_HPP
