// Verified with: https://judge.yosupo.jp/problem/sqrt_mod
// Details: https://judge.yosupo.jp/submission/319863

#include <iostream>
#include <atcoder/modint>
#include <kotone/modint_utility>

using mint = atcoder::modint;

int main() {
    int T;
    std::cin >> T;
    while (T--) {
        int Y, P;
        std::cin >> Y >> P;
        mint::set_mod(P);
        mint result = kotone::sqrt_mint(mint(Y));
        if (Y != 0 && result == 0) std::cout << -1 << std::endl;
        else std::cout << result.val() << std::endl;
    }
}
