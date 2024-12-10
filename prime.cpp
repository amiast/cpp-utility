/**
 * prime.cpp
 */

#include "prime.h"
#include <type_traits>

template <typename T>
std::vector<bool> sieve(T n) {
    static_assert(std::is_integral_v<T>, "Integral required.");

    std::vector<bool> is_prime(n + 1, true);
    is_prime.at(0) = false;
    is_prime.at(1) = false;

    for (T i = 2; i <= n; i++) {
        if (!is_prime.at(i)) continue;

        for (T j = i * i; j <= n; j += i) is_prime.at(j) = false;
    }

    return is_prime;
}
