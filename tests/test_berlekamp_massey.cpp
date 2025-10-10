// Verified with: https://judge.yosupo.jp/problem/find_linear_recurrence
// Details: https://judge.yosupo.jp/submission/319760

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/berlekamp_massey>

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
    std::vector<mint> recurrence = kotone::berlekamp_massey(A);
    std::cout << recurrence.size() << std::endl;
    for (mint &r : recurrence) std::cout << r.val() << ' ';
    std::cout << std::endl;
}
