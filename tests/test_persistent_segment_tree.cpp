// Verified with: https://judge.yosupo.jp/problem/persistent_range_affine_range_sum
// Details: https://judge.yosupo.jp/submission/365817

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/persistent_segment_tree>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
S e() { return {0, 0}; }
S mapping(S f, S x) { return {f.first * x.first + f.second * x.second, x.second}; }
S composition(S f, S g) { return {f.first * g.first, f.first * g.second + f.second}; }
S id() { return {1, 0}; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> ref(Q + 1);
    kotone::persistent_segment_tree<S, op, e, S, mapping, composition, id> seg(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        ref[0] = seg.set(ref[0], i, {a, 1});
    }
    for (int i = 1; i <= Q; i++) {
        int t, k;
        std::cin >> t >> k;
        k++;
        if (t == 0) {
            int l, r, b, c;
            std::cin >> l >> r >> b >> c;
            ref[i] = seg.apply(ref[k], l, r, {b, c});
        } else if (t == 1) {
            int s, l, r;
            std::cin >> s >> l >> r;
            s++;
            ref[i] = seg.copy_paste(ref[s], ref[k], l, r);
        } else {
            int l, r;
            std::cin >> l >> r;
            std::cout << seg.prod(ref[k], l, r).first.val() << std::endl;
        }
    }
}
