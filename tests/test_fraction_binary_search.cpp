// Verified with: https://judge.yosupo.jp/problem/rational_approximation
// Details: https://judge.yosupo.jp/submission/378608

#include <iostream>
#include <kotone/fraction>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        int64_t N, x, y;
        std::cin >> N >> x >> y;
        auto leq = [&](const std::pair<int64_t, int64_t> &frac) {
            return frac.first * y <= frac.second * x;
        };
        auto lt = [&](const std::pair<int64_t, int64_t> &frac) {
            return frac.first * y < frac.second * x;
        };
        auto [a, b] = kotone::binary_search_rational(leq, N).first;
        auto [c, d] = kotone::binary_search_rational(lt, N).second;
        std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
    }
}
