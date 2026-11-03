// Verified with: https://judge.yosupo.jp/problem/matrix_rank
// Details: https://judge.yosupo.jp/submission/414955

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
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
    std::cout << kotone::matrix::basis(A).size() << std::endl;
}
