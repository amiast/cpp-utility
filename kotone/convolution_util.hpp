#ifndef KOTONE_CONVOLUTION_UTIL_HPP
#define KOTONE_CONVOLUTION_UTIL_HPP 1

#include <vector>
#include <algorithm>
#include <atcoder/convolution>

namespace kotone {

// Performs naive convolution of the two given formal power series.
// If either `fps_l` or `fps_r` is empty, returns an empty vector.
template <typename T> std::vector<T> naive_convolution(const std::vector<T> &fps_l, const std::vector<T> &fps_r) {
    assert(fps_l.size() <= 100000000u);
    assert(fps_r.size() <= 100000000u);
    if (fps_l.empty() || fps_r.empty()) return {};
    std::vector<T> result(fps_l.size() + fps_r.size() - 1);
    for (unsigned i{}; i < fps_l.size(); i++) {
        for (unsigned j{}; j < fps_r.size(); j++) {
            result[i + j] += fps_l[i] * fps_r[j];
        }
    }
    return result;
}

// Returns the inverse of the formal power series up to the first `n` coefficients.
// Requires `!fps.empty() && fps[0] != 0`.
// Requires `0 <= n <= 100000000`.
template <typename mint> std::vector<mint> inv_fps(const std::vector<mint> &fps, int n) {
    assert(0 <= n && n <= 100000000);
    assert(!fps.empty() && fps[0] != 0);
    assert(fps.size() <= 100000000u);
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

// Returns the derivative of the formal power series.
// Returns an empty vector if `fps` is empty.
template <typename mint> std::vector<mint> derivative(const std::vector<mint> &fps) {
    if (fps.empty()) return {};
    assert(fps.size() <= 100000000u);
    std::vector<mint> result(fps.begin() + 1, fps.end());
    for (unsigned i{}; i < result.size(); i++) result[i] *= i + 1;
    return result;
}

// Returns the integral of the formal power series.
// Sets the integral's coefficient of the term independent of variables to `0`.
// If `fps` is empty, returns an empty vector.
template <typename mint> std::vector<mint> integral(const std::vector<mint> &fps) {
    if (fps.empty()) return {};
    assert(fps.size() <= 100000000u);
    int len = static_cast<int>(fps.size());
    std::vector<mint> result(len + 1);
    result[1] = 1;
    for (int i = 2; i <= len; i++) {
        result[i] = -result[mint::mod() % i] * mint(mint::mod() / i);
    }
    for (int i = 0; i < len; i++) result[i + 1] *= fps[i];
    return result;
}

// Returns the logarithm of the formal power series up to the first `n` coefficients.
// Requires `fps` to be non-empty and `fps[0] == 1`.
// Requires `0 <= n <= 100000000`.
template <typename mint> std::vector<mint> log_fps(const std::vector<mint> &fps, int n) {
    assert(!fps.empty());
    assert(fps[0] == 1);
    assert(fps.size() <= 100000000u);
    assert(0 <= n && n <= 100000000);
    if (n == 0) return {};
    std::vector<mint> dfps = derivative(fps), ifps = inv_fps(fps, n);
    std::vector<mint> prod = atcoder::convolution(dfps, ifps);
    prod.resize(n - 1);
    std::vector<mint> result = integral(prod);
    result.resize(n);
    return result;
}

// Returns the exponential of the formal power series up to the first `n` coefficients.
// If `fps` is empty, returns a vector of `n` elements filled with `0`.
// Requires `fps[0] == 0` if `fps` is not empty.
// Requires `0 <= n <= 100000000`.
template <typename mint> std::vector<mint> exp_fps(const std::vector<mint> &fps, int n) {
    assert(fps.size() <= 100000000u);
    assert(0 <= n && n <= 100000000);
    if (fps.empty()) return std::vector<mint>(n);
    assert(fps[0] == 0);
    if (n == 0) return {};
    std::vector<mint> result{1};
    int m = 1;
    while (m < n) {
        m = std::min(m * 2, n);
        std::vector<mint> log = log_fps(result, m);
        for (int i = 0; i < m; i++) {
            log[i] = -log[i];
            if (i < static_cast<int>(fps.size())) log[i] += fps[i];
        }
        log[0] += 1;
        result = atcoder::convolution(result, log);
        result.resize(m);
    }
    return result;
}

// Returns the formal power series raised to the specified power up to the first `n` coefficients.
// If `pow == 0`, the first element of the resulting vector is `1` and subsequence elements are `0`.
// If `pow > 0` and `fps` is empty, returns a vector of `n` elements filled with `0`.
// Requires `pow >= 0`.
// Requires `0 <= n <= 100000000`.
template <typename mint> std::vector<mint> pow_fps(const std::vector<mint> &fps, int64_t pow, int n) {
    assert(fps.size() <= 100000000u);
    assert(pow >= 0);
    assert(0 <= n && n <= 100000000);
    if (n == 0) return {};
    if (pow == 0) {
        std::vector<mint> result(n);
        result[0] = 1;
        return result;
    }
    int len = static_cast<int>(fps.size());
    int d = -1;
    for (int i = 0; i < len; i++) {
        if (fps[i] == 0) continue;
        d = i;
        break;
    }
    if (d == -1 || d > (n - 1) / pow) return std::vector<mint>(n);
    mint lead_coeff = fps[d];
    mint lead_pow = lead_coeff.pow(pow);
    mint lead_inv = 1 / lead_coeff;
    std::vector<mint> truncated_fps(fps.begin() + d, fps.end());
    for (mint &c : truncated_fps) c *= lead_inv;
    truncated_fps = log_fps(truncated_fps, n - d * pow);
    for (mint &c : truncated_fps) c *= pow;
    truncated_fps = exp_fps(truncated_fps, n - d * pow);
    for (mint &c : truncated_fps) c *= lead_pow;
    std::vector<mint> result(n);
    for (int i = 0; i + d * pow < n; i++) result[i + d * pow] = truncated_fps[i];
    return result;
}

// Computes term `a[k]` of a homogeneous linear recurrence `a` of order `n`.
// More specifically, `recurrence` specifies coefficients `c` in the relation
// `a[i] = c[0] * a[i - 1] + ... + c[n - 1] * a[i - n]`.
// Returns `0` if either `recurrence` or `init` is empty.
// If `init` contains `d` terms and `d < n`, assumes initial values of `0` for `d + 1, ..., n - 1`.
// Requires `d <= n`.
// Requires `k >= 0`.
template <typename mint> mint solve_recurrence(const std::vector<mint> &recurrence, const std::vector<mint> &init, int64_t k) {
    assert(k >= 0);
    if (init.empty()) return 0;
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
        numerator = num_next;
        denominator = denom_next;
        k >>= 1;
    }

    return numerator[0] / denominator[0];
}

}  // namespace kotone

#endif  // KOTONE_CONVOLUTION_UTIL_HPP
