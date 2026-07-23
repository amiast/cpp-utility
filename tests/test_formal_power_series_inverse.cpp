// Verified with: https://judge.yosupo.jp/problem/inv_of_formal_power_series
// Details: https://judge.yosupo.jp/submission/387454

#include <iostream>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N;
    std::cin >> N;
    fps A(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        A[i] = a;
    }
    fps B = fps::inverse(A, N);
    for (mint b : B) std::cout << b.val() << ' ';
    std::cout << std::endl;
}
