// Verified with: https://judge.yosupo.jp/problem/pow_of_formal_power_series
// Details: https://judge.yosupo.jp/submission/319803

#include <iostream>
#include <vector>
#include <kotone/convolution_util>

using mint = atcoder::modint998244353;

int main() {
    int N;
    int64_t M;
    std::cin >> N >> M;
    std::vector<mint> A(N);
    for (mint &a : A) {
        int i;
        std::cin >> i;
        a = i;
    }
    std::vector<mint> B = kotone::pow_fps(A, M, N);
    for (mint &b : B) std::cout << b.val() << ' ';
    std::cout << std::endl;
}
