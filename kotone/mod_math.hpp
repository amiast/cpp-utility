#ifndef KOTONE_MOD_MATH_HPP
#define KOTONE_MOD_MATH_HPP 1

#include <vector>
#include <concepts>
#include <cassert>
#include <atcoder/convolution>

namespace kotone {

// Returns the non-negative remainder produced from dividing `n` by `m`.
// Requires `m > 0`.
template <std::signed_integral T> uint64_t mod(T n, uint64_t m) {
    assert(m > 0u);
    if (n >= 0) return uint64_t(n) % m;
    if (m < 1ULL << 63) {
        int64_t sm = m;
        return (n % sm + sm) % sm;
    }
    if (n == -1LL << 63) return m - (1ULL << 63);
    return m - -n;
}

// Returns the non-negative remainder produced from dividing `n` by `m`.
// Requires `m > 0`.
template <std::unsigned_integral T> uint64_t mod(T n, uint64_t m) {
    assert(m > 0u);
    return n % m;
}

// Returns `(a + b) % m`.
// Requires `m > 0`.
template <std::integral S, std::integral T> uint64_t sum_mod(S a, T b, uint64_t m) {
    uint64_t ua = mod(a, m), ub = mod(b, m);
    uint64_t result = ua + ub;
    if (result < ua) result -= m;
    if (result >= m) result -= m;
    return result;
}

// Returns `a * b % m`.
// Requires `m > 0`.
// Requires compiler-provided type `__int128_t`.
template <std::integral S, std::integral T> uint64_t prod_mod(S a, T b, uint64_t m) {
    uint64_t ua = mod(a, m), ub = mod(b, m);
    return __int128_t(ua) * ub % m;
}

// Returns `pow(n, k) % m`.
// Returns `1 % m` if `n == k == 0`.
// Requires `m > 0`.
// Requires compiler-provided type `__int128_t`.
template <std::integral T> uint64_t pow_mod(T n, uint64_t k, uint64_t m) {
    assert(m > 0u);
    uint64_t un = mod(n, m);
    uint64_t result = 1 % m;
    while (k) {
        if (k & 1u) result = prod_mod(result, un, m);
        un = prod_mod(un, un, m);
        k >>= 1;
    }
    return result;
}

// Returns the convolution of `a` and `b` modulo `m`.
// If `a.empty() || b.empty()`, returns an empty vector.
// Requires `m < 1 << 32`.
// Requires compiler-provided type `__int128_t`.
template <std::integral S, std::integral T>
std::vector<uint64_t> convolution_mod(const std::vector<S> &a, const std::vector<T> &b, uint64_t m) {
    constexpr uint64_t m0 = 167772161, m1 = 469762049, m2 = 1224736769;
    using mint0 = atcoder::static_modint<m0>;
    using mint1 = atcoder::static_modint<m1>;
    using mint2 = atcoder::static_modint<m2>;
    assert(m < 1ULL << 32);
    if (a.empty() || b.empty()) return {};
    int na = a.size(), nb = b.size();
    std::vector<mint0> a0(na), b0(nb);
    std::vector<mint1> a1(na), b1(nb);
    std::vector<mint2> a2(na), b2(nb);
    for (int i = 0; i < na; i++) a0[i] = a[i], a1[i] = a[i], a2[i] = a[i];
    for (int i = 0; i < nb; i++) b0[i] = b[i], b1[i] = b[i], b2[i] = b[i];
    std::vector<mint0> c0 = atcoder::convolution(a0, b0);
    std::vector<mint1> c1 = atcoder::convolution(a1, b1);
    std::vector<mint2> c2 = atcoder::convolution(a2, b2);
    int inv0 = mint1(m0).inv().val();
    int inv01 = mint2(prod_mod(m0, m1, m2)).inv().val();
    std::vector<uint64_t> result(na + nb - 1);
    for (int i = 0; i < na + nb - 1; i++) {
        int t1 = (mint1(c1[i].val() - c0[i].val()) * inv0).val();
        int x = (prod_mod(m0, t1, m2) + c0[i].val()) % m2;
        int t2 = (mint2(c2[i].val() - x) * inv01).val();
        result[i] = (c0[i].val() + m0 * t1 + prod_mod(m0 * m1, t2, m)) % m;
    }
    return result;
}

}  // namespace kotone

#endif  // KOTONE_MOD_MATH_HPP
