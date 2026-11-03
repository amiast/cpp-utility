// Verified with: https://judge.yosupo.jp/problem/pow_of_matrix
// Details: https://judge.yosupo.jp/submission/414953

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N;
    int64_t K;
    std::cin >> N >> K;
    std::vector A(N, std::vector<mint>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int a;
            std::cin >> a;
            A[i][j] = a;
        }
    }
    std::vector<std::vector<mint>> B = kotone::matrix::pow(A, K);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << B[i][j].val() << ' ';
        }
        std::cout << '\n';
    }
}
