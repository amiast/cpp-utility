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
template <std::integral S, std::integral T> uint64_t prod_mod(S a, T b, uint64_t m) {
    uint64_t ua = mod(a, m), ub = mod(b, m);
    if (m <= 1ULL << 32) return ua * ub % m;
    if (ua < ub) std::swap(ua, ub);
    uint64_t result = 0;
    while (ub) {
        if (ub & 1u) result = sum_mod(result, ua, m);
        ua = sum_mod(ua, ua, m);
        ub >>= 1;
    }
    return result;
}

// Returns `pow(n, k) % m`.
// Returns `1 % m` if `n == k == 0`.
// Requires `m > 0`.
template<std::integral T> uint64_t pow_mod(T n, uint64_t k, uint64_t m) {
    assert(m > 0u);
    if (k == 0) return m > 1u;
    uint64_t un = mod(n, m);
    uint64_t result = 1;
    while (k) {
        if (k & 1u) result = prod_mod(result, un, m);
        un = prod_mod(un, un, m);
        k >>= 1;
    }
    return result;
}

}

#endif  // KOTONE_MOD_MATH_HPP
