#ifndef KOTONE_MOD_MATH_HPP
#define KOTONE_MOD_MATH_HPP 1

#include <concepts>
#include <cassert>

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
// Requires compiler-provided type `__int128`.
template <std::integral S, std::integral T> uint64_t prod_mod(S a, T b, uint64_t m) {
    uint64_t ua = mod(a, m), ub = mod(b, m);
    return __int128(ua) * ub % m;
}

// Returns `pow(n, k) % m`.
// Returns `1 % m` if `n == k == 0`.
// Requires `m > 0`.
// Requires compiler-provided type `__int128`.
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

}

#endif  // KOTONE_MOD_MATH_HPP
