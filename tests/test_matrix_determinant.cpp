// Verified with: https://judge.yosupo.jp/problem/matrix_det
// Details: https://judge.yosupo.jp/submission/414954

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/matrix>

using mint = atcoder::modint998244353;

int main() {
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
    std::cout << kotone::matrix::det(A).val() << std::endl;
}
