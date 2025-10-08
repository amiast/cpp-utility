// Verified with: https://judge.yosupo.jp/problem/exp_of_formal_power_series
// Details: https://judge.yosupo.jp/submission/319381

#include <iostream>
#include <vector>
#include <kotone/convolution_util>

using mint = atcoder::modint998244353;

int main() {
    int N;
    std::cin >> N;
    std::vector<mint> A(N);
    for (mint &a : A) {
        int i;
        std::cin >> i;
        a = i;
    }
    std::vector<mint> B = kotone::exp_fps(A, N);
    for (mint &b : B) std::cout << b.val() << ' ';
    std::cout << std::endl;
}
