#ifndef KOTONE_PRIME_HPP
#define KOTONE_PRIME_HPP 1

#include <vector>
#include <algorithm>
#include <bit>
#include <numeric>
#include <cassert>
#include <kotone/random>
#include <kotone/mod_math>

namespace kotone {

// Retuens a `bool` indicating whether `n` is prime.
// Requires compiler-provided type `__int128`.
bool is_prime(int64_t n) {
    constexpr static int bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n <= 2) return n == 2;
    if (~n & 1) return false;
    int64_t s = std::bit_width(uint64_t((n - 1) & -(n - 1))) - 1;
    int64_t d = (n - 1) >> s;
    for (int a : bases) {
        if (a >= n) continue;
        int64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool flag = true;
        for (int t = 0; t < s - 1; t++) {
            x = prod_mod(x, x, n);
            if (x == n - 1) {
                flag = false;
                break;
            }
        }
        if (flag) return false;
    }
    return true;
}

// Returns a factor of composite number `n`.
// Requires `n` to be a positive composite number.
// Requires compiler-provided type `__int128`.
int64_t pollards_rho(int64_t n) {
    assert(n >= 4);
    if (n % 2 == 0) return 2;
    while (true) {
        int64_t b = randint() % (n - 3) + 1;
        auto g = [n, b](int64_t x) {
            return sum_mod(prod_mod(x, x, n), b, n);
        };
        int64_t x = randint() % n;
        int64_t y = x, d = 1;
        while (d == 1) {
            x = g(x);
            y = g(g(y));
            d = std::gcd(std::abs(x - y), n);
        }
        if (d != n) return d;
    }
}

// Returns a vector containing all prime factors of `n`.
// The order of factors is undefined.
// Requires `n` to be a positive integer.
// Requires compiler-provided type `__int128`.
std::vector<int64_t> factorize(int64_t n) {
    assert(n > 0);
    auto rec = [&](auto &rec, int64_t n, std::vector<int64_t> &vec) {
        if (n == 1) return;
        if (is_prime(n)) {
            vec.push_back(n);
            return;
        }
        int64_t d = pollards_rho(n);
        rec(rec, d, vec);
        rec(rec, n / d, vec);
    };
    std::vector<int64_t> result;
    rec(rec, n, result);
    return result;
}

// Returns the least primitive root of prime `p`.
// Requires `p` to be a prime.
// Requires compiler-provided type `__int128`.
int64_t least_primitive_root(int64_t p) {
    std::vector<int64_t> factors = factorize(p - 1);
    std::sort(factors.begin(), factors.end());
    factors.erase(std::unique(factors.begin(), factors.end()), factors.end());
    for (int64_t n = 1; n < p; n++) {
        bool flag = true;
        for (int64_t q : factors) {
            if (pow_mod(n, (p - 1) / q, p) == 1) {
                flag = false;
                break;
            }
        }
        if (flag) return n;
    }
}

}  // namespace kotone

#endif  // KOTONE_PRIME_HPP
