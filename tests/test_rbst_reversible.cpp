// Verified with: https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
// Details: https://judge.yosupo.jp/submission/405965

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/rbst_reversible>

using mint = atcoder::modint998244353;
using S = std::pair<mint, int>;
S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
S e() { return {0, 0}; }
using F = std::pair<mint, mint>;
S mapping(F f, S x) { return {f.first * x.first + f.second * x.second, x.second}; }
F composition(F f, F g) { return {f.first * g.first, f.first * g.second + f.second}; }
F id() { return {1, 0}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<S> vec(N);
    for (auto &[x, c] : vec) {
        int a;
        std::cin >> a;
        x = a;
        c = 1;
    }

    int root = 0;
    kotone::reversible_randomized_binary_search_tree<S, op, e, F, mapping, composition, id, false> rbst(root, vec);

    while (Q--) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int i, x;
            std::cin >> i >> x;
            auto [L, R] = rbst.split(root, i);
            int N = rbst.make_node({x, 1});
            root = rbst.merge(rbst.merge(L, N), R);
        } else if (t == 1) {
            int i;
            std::cin >> i;
            auto [M, R] = rbst.split(root, i + 1);
            int L = rbst.split(M, i).first;
            root = rbst.merge(L, R);
        } else if (t == 2) {
            int l, r;
            std::cin >> l >> r;
            auto [MR, R] = rbst.split(root, r);
            auto [L, ML] = rbst.split(MR, l);
            rbst.reverse(ML);
            root = rbst.merge(rbst.merge(L, ML), R);
        } else if (t == 3) {
            int l, r, b, c;
            std::cin >> l >> r >> b >> c;
            auto [MR, R] = rbst.split(root, r);
            auto [L, ML] = rbst.split(MR, l);
            rbst.apply(ML, {b, c});
            root = rbst.merge(rbst.merge(L, ML), R);
        } else {
            int l, r;
            std::cin >> l >> r;
            auto [MR, R] = rbst.split(root, r);
            auto [L, ML] = rbst.split(MR, l);
            std::cout << rbst.prod(ML).first.val() << '\n';
            root = rbst.merge(rbst.merge(L, ML), R);
        }
    }
}
