// Verified with: https://judge.yosupo.jp/problem/gcd_convolution
// Details: https://judge.yosupo.jp/submission/413531

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/gcd_convolution>

using mint = atcoder::modint998244353;

int main() {
    int N;
    std::cin >> N;
    std::vector<mint> A(N + 1), B(N + 1);
    for (int i = 1; i <= N; i++) {
        int a;
        std::cin >> a;
        A[i] = a;
    }
    for (int i = 1; i <= N; i++) {
        int b;
        std::cin >> b;
        B[i] = b;
    }
    std::vector<mint> C = kotone::divisor_convolution<mint, true>(std::move(A), std::move(B));
    for (int i = 1; i <= N; i++) std::cout << C[i].val() << ' ';
    std::cout << std::endl;
}
