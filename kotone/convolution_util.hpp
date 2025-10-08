#ifndef KOTONE_CONVOLUTION_UTIL_HPP
#define KOTONE_CONVOLUTION_UTIL_HPP 1

#include <vector>
#include <algorithm>
#include <atcoder/convolution>

namespace kotone {

// Returns the inverse of the formal power series up to the first `n` coefficients.
// Requires `!fps.empty() && fps[0] != 0`.
// Requires `0 <= n <= 100000000`.
template <typename mint> std::vector<mint> inv_fps(const std::vector<mint> &fps, int n) {
    assert(!fps.empty() && fps[0] != 0);
    assert(0 <= n && n <= 100000000);
    if (n == 0) return {};
    std::vector<mint> result{1 / fps[0]};
    int m = 1;
    int len = static_cast<int>(fps.size());
    while (m < n) {
        m = std::min(m * 2, n);
        std::vector<mint> sub(fps.begin(), fps.begin() + std::min(m, len));
        if (m > len) sub.resize(m);
        std::vector<mint> prod = atcoder::convolution(result, sub);
        prod.resize(m);
        prod[0] = 2 - prod[0];
        for (int i = 1; i < m; i++) prod[i] = -prod[i];
        result = atcoder::convolution(result, prod);
        result.resize(m);
    }
    result.resize(n);
    return result;
}

// Computes term `a[k]` of a homogeneous linear recurrence `a` of order `n`.
// More specifically, `recurrence` specifies coefficients `c` in the relation
// `a[i] = c[0] * a[i - 1] + ... + c[n - 1] * a[i - n]`.
// Returns `0` if either `recurrence` or `init` is empty.
// If `init` contains `d` terms and `d < n`, assumes initial values of `0` for `d + 1, ..., n - 1`.
// Requires `d <= n`.
// Requires `k >= 0`.
template <typename mint> mint solve_recurrence(const std::vector<mint> &recurrence, int64_t k, const std::vector<mint> &init) {
    assert(k >= 0);
    if (recurrence.empty() || init.empty()) return 0;
    assert(init.size() <= recurrence.size() && recurrence.size() <= 100000000u);
    if (k < static_cast<int>(init.size())) return init[k];
    int n = static_cast<int>(recurrence.size());
    bool all_zero = true;
    for (int i = 0; i < n; i++) {
        if (recurrence[i] != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) return 0;

    std::vector<mint> denominator(n + 1, 1);
    for (int i = 0; i < n; i++) denominator[i + 1] = -recurrence[i];
    std::vector<mint> numerator = atcoder::convolution(init, denominator);
    numerator.resize(n);

    while (k) {
        std::vector<mint> denom_alt = denominator;
        for (int i = 1; i <= n; i += 2) {
            denom_alt[i] = -denom_alt[i];
        }
        numerator = atcoder::convolution(numerator, denom_alt);
        denominator = atcoder::convolution(denominator, denom_alt);

        std::vector<mint> num_next(n), denom_next(n + 1);
        for (int i = 0; i < n; i++) num_next[i] = numerator[i * 2 + (k & 1)];
        for (int i = 0; i <= n; i++) denom_next[i] = denominator[i * 2];
        numerator.swap(num_next);
        denominator.swap(denom_next);
        k >>= 1;
    }

    return numerator[0] / denominator[0];
}

}  // namespace kotone

#endif  // KOTONE_CONVOLUTION_UTIL_HPP
