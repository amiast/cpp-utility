// Verified with: https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod
// Details: https://judge.yosupo.jp/submission/393779

#include <iostream>
#include <atcoder/modint>
#include <kotone/modint_utility>

using mint = atcoder::modint;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T, m;
    std::cin >> T >> m;
    mint::set_mod(m);
    kotone::modint_utility<mint> util;
    auto comb = [&](int n, int k) -> mint {
        if (0 <= k && k <= n) return util.comb(n, k);
        return 0;
    };
    while (T--) {
        int n, k;
        std::cin >> n >> k;
        std::cout << comb(n, k).val() << '\n';
    }
}
