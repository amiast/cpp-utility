// Verified with: https://judge.yosupo.jp/problem/pow_of_formal_power_series
// Details: https://judge.yosupo.jp/submission/387461

#include <iostream>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N;
    int64_t M;
    std::cin >> N >> M;
    fps A(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        A[i] = a;
    }
    fps B = fps::power(A, M, N);
    for (mint b : B) std::cout << b.val() << ' ';
    std::cout << std::endl;
}
