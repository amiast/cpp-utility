// Verified with: https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence
// Details: https://judge.yosupo.jp/submission/387462

#include <iostream>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int64_t d, k;
    std::cin >> d >> k;
    fps A(d), C(d);
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
    std::cout << fps::solve_recurrence(C, A, k).val() << std::endl;
}
