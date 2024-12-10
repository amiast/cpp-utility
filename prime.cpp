/**
 * prime.cpp
 */

#include "prime.h"
#include <type_traits>

std::vector<bool> sieve(int n) {
    std::vector<bool> is_prime(n + 1, true);
    is_prime.at(0) = false;
    is_prime.at(1) = false;

    for (int i = 2; i <= n; i++) {
        if (!is_prime.at(i)) continue;

        for (int j = i * i; j <= n; j += i) is_prime.at(j) = false;
    }

    return is_prime;
}
