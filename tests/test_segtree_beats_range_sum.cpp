// Verified with: https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum
// Details: https://judge.yosupo.jp/submission/318866

#include <iostream>
#include <vector>
#include <kotone/segtree_beats>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int64_t> A(N);
    for (int64_t &a : A) std::cin >> a;
    kotone::segtree_beats beats(A);
    while (Q--) {
        int t, l, r;
        std::cin >> t >> l >> r;
        int64_t b;
        if (t != 3) std::cin >> b;
        if (t == 0) beats.chmin(l, r, b);
        else if (t == 1) beats.chmax(l, r, b);
        else if (t == 2) beats.add(l, r, b);
        else std::cout << beats.sum(l, r) << std::endl;
    }
}
