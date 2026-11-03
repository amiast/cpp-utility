// Verified with: https://judge.yosupo.jp/problem/matrix_product
// Details: https://judge.yosupo.jp/submission/414952

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, M, K;
    std::cin >> N >> M >> K;
    std::vector A(N, std::vector<mint>(M)), B(M, std::vector<mint>(K));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int a;
            std::cin >> a;
            A[i][j] = a;
        }
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            int b;
            std::cin >> b;
            B[i][j] = b;
        }
    }
    std::vector<std::vector<mint>> C = kotone::matrix::prod(A, B);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            std::cout << C[i][j].val() << ' ';
        }
        std::cout << '\n';
    }
}
