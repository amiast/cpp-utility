// Verified with: https://judge.yosupo.jp/problem/inverse_matrix
// Details: https://judge.yosupo.jp/submission/414956

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N;
    std::cin >> N;
    std::vector A(N, std::vector<mint>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int a;
            std::cin >> a;
            A[i][j] = a;
        }
    }
    auto [B, found] = kotone::matrix::inv(A);
    if (!found) {
        std::cout << "-1\n";
        return 0;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B[i][j].val() << ' ';
        }
        std::cout << '\n';
    }
}
