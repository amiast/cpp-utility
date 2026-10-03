// Verified with: https://judge.yosupo.jp/problem/convolution_mod_1000000007
// Details: https://judge.yosupo.jp/submission/407627

#include <iostream>
#include <vector>
#include <kotone/mod_math>

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<uint64_t> A(N), B(M);
    for (uint64_t &a : A) std::cin >> a;
    for (uint64_t &b : B) std::cin >> b;
    std::vector<uint64_t> C = kotone::convolution_mod(A, B, 1000000007);
    for (uint64_t &c : C) std::cout << c << ' ';
    std::cout << std::endl;
}
