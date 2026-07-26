// Verified with: https://judge.yosupo.jp/problem/division_of_polynomials
// Details: https://judge.yosupo.jp/submission/388204

#include <iostream>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N, M;
    std::cin >> N >> M;
    fps F(N), G(M);
    for (int i = 0; i < N; i++) {
        int f;
        std::cin >> f;
        F[i] = f;
    }
    for (int i = 0; i < M; i++) {
        int g;
        std::cin >> g;
        G[i] = g;
    }
    auto [Q, R] = fps::divmod(F, G);
    Q.strip(), R.strip();
    std::cout << Q.size() << ' ' << R.size() << std::endl;
    for (mint &q : Q) std::cout << q.val() << ' ';
    std::cout << std::endl;
    for (mint &r : R) std::cout << r.val() << ' ';
    std::cout << std::endl;
}
