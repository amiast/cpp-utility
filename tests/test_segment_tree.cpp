// Verified with: https://judge.yosupo.jp/problem/point_set_range_composite_large_array
// Details: https://judge.yosupo.jp/submission/328099

#include <iostream>
#include <utility>
#include <atcoder/modint>
#include <kotone/segment_tree>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S a, S b) { return {b.first * a.first, b.first * a.second + b.second}; }
S e() { return {1, 0}; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::segment_tree<S, op, e> seg(N);
    while (Q--) {
        int t, l, r, x;
        std::cin >> t >> l >> r >> x;
        if (t == 0) seg.set(l, {r, x});
        else {
            S prod = seg.prod(l, r);
            mint result = prod.first * x + prod.second;
            std::cout << result.val() << std::endl;
        }
    }
}
