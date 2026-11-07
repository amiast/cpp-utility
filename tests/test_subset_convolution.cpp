// Verifies with: https://judge.yosupo.jp/problem/subset_convolution
// Details: https://judge.yosupo.jp/submission/339447

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/subset_convolution>

using mint = atcoder::modint998244353;

int main() {
    int N;
    std::cin >> N;
    std::vector<mint> A(1 << N), B(1 << N);
    for (mint &a : A) {
        int x;
        std::cin >> x;
        a = x;
    }
    for (mint &b : B) {
        int x;
        std::cin >> x;
        b = x;
    }
    using conv = kotone::subset_convolution<mint, 20>;
    std::vector<mint> C = conv::convolve(A, B);
    for (mint &c : C) std::cout << c.val() << ' ';
    std::cout << std::endl;
}
