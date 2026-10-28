// Verified with: https://judge.yosupo.jp/problem/factorize
// Details: https://judge.yosupo.jp/submission/413756

#include <iostream>
#include <kotone/prime>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        int64_t a;
        std::cin >> a;
        std::vector<int64_t> factors = kotone::factorize(a);
        std::sort(factors.begin(), factors.end());
        std::cout << factors.size();
        for (int64_t p : factors) std::cout << ' ' << p;
        std::cout << '\n';
    }
}
