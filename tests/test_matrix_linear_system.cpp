// Verified with: https://judge.yosupo.jp/problem/system_of_linear_equations
// Details: https://judge.yosupo.jp/submission/417149

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, M;
    std::cin >> N >> M;
    std::vector A(N, std::vector<mint>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int a;
            std::cin >> a;
            A[i][j] = a;
        }
    }
    std::vector<mint> B(N);
    for (int i = 0; i < N; i++) {
        int b;
        std::cin >> b;
        B[i] = b;
    }
    auto [sol, space] = kotone::matrix::solve(A, B);
    if (sol.empty()) {
        std::cout << "-1\n";
        return 0;
    }
    std::cout << space.size() << '\n';
    for (mint &m : sol) std::cout << m.val() << ' ';
    std::cout << '\n';
    for (auto &s : space) {
        for (mint &m : s) std::cout << m.val() << ' ';
        std::cout << '\n';
    }
}
