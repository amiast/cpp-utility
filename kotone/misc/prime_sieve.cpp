// #include <kotone/misc/prime_sieve.cpp>
// https://github.com/amiast/cpp-utility

#include <vector>

int main() {
    int M = 32000;
    std::vector<int> lpf(M + 1);
    for (int i = 0; i <= M; i++) lpf[i] = i;
    std::vector<int> primes;
    for (int p = 2; p <= M; p++) {
        if (lpf[p] == p) primes.push_back(p);
        for (int q : primes) {
            if (p > M / q || lpf[p] < q) break;
            lpf[p * q] = q;
        }
    }
}
