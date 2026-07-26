// Verified with: https://judge.yosupo.jp/problem/polynomial_interpolation
// Details: https://judge.yosupo.jp/submission/388226

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N;
    std::cin >> N;
    std::vector<std::pair<mint, mint>> vec(N);
    for (int i = 0; i < N; i++) {
        int x;
        std::cin >> x;
        vec[i].first = x;
    }
    for (int i = 0; i < N; i++) {
        int y;
        std::cin >> y;
        vec[i].second = y;
    }
    for (mint &m : fps::interpolate(vec)) std::cout << m.val() << ' ';
    std::cout << std::endl;
}
