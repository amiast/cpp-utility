// Verified with: https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
// Details: https://judge.yosupo.jp/submission/319383

#include <iostream>
#include <vector>
#include <kotone/convolution_util>

using mint = atcoder::modint998244353;

int main() {
    int64_t d, k;
    std::cin >> d >> k;
    std::vector<mint> A(d), C(d);
    for (mint &a : A) {
        int i;
        std::cin >> i;
        a = i;
    }
    for (mint &c : C) {
        int i;
        std::cin >> i;
        c = i;
    }
    std::cout << kotone::solve_recurrence(C, k, A).val() << std::endl;
}
