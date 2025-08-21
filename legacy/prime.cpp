/**
 * prime.cpp
 */

#include "prime.h"

std::vector<int> sieve(int n) {
    std::vector<int> primes;
    std::vector<int> factor(n + 1, 1);
    for (int i = 2; i < n; i++) {
        if (factor.at(i) == 1) {
            primes.push_back(i);
            factor.at(i) = i;
        }
        for (int p : primes) {
            if (i * p > n || p > factor.at(i)) {
                break;
            }
            factor.at(i * p) = p;
        }
    }
    return primes;
}
