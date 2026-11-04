// Verified with: https://judge.yosupo.jp/problem/characteristic_polynomial
// Details: https://judge.yosupo.jp/submission/415186

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
    for (mint &m : kotone::matrix::charpoly(A)) std::cout << m.val() << ' ';
    std::cout << std::endl;
}
