// Verified with: https://judge.yosupo.jp/problem/range_affine_range_sum_large_array
// Details: https://judge.yosupo.jp/submission/417587

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/persistent_segment_tree>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
S e() { return {0, 0}; }
S to_node(int64_t l, int64_t r) { return {0, r - l}; }
S mapping(S f, S x) { return {f.first * x.first + f.second * x.second, x.second}; }
S composition(S f, S g) { return {f.first * g.first, f.first * g.second + f.second}; }
S id() { return {1, 0}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    kotone::persistent_segment_tree<S, op, e, to_node, S, mapping, composition, id, false> seg(N);
    while (Q--) {
        int t, l, r;
        std::cin >> t >> l >> r;
        if (t == 0) {
            int b, c;
            std::cin >> b >> c;
            seg.apply(0, l, r, {b, c});
        } else {
            std::cout << seg.prod(0, l, r).first.val() << '\n';
        }
    }
}
