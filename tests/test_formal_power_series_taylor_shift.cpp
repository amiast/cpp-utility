// Verified with: https://judge.yosupo.jp/problem/polynomial_taylor_shift
// Details: https://judge.yosupo.jp/submission/397600

#include <iostream>
#include <vector>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N, c;
    std::cin >> N >> c;
    fps A(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        A[i] = a;
    }
    for (mint &m : fps::taylor_shift(A, c)) std::cout << m.val() << ' ';
    std::cout << std::endl;
}
