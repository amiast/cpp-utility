// Verified with: https://judge.yosupo.jp/problem/vertex_set_path_composite
// Details: https://judge.yosupo.jp/submission/401517

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <atcoder/segtree>
#include <kotone/heavy_light_decomposition>

using mint = atcoder::modint998244353;
using affine = std::pair<mint, mint>;
affine op(affine p, affine u) { return {u.first * p.first, u.first * p.second + u.second}; }
affine op_rev(affine p, affine u) { return op(u, p); }
affine e() { return {1, 0}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<affine> init(N);
    for (int i = 0; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        init[i].first = a, init[i].second = b;
    }
    std::vector<std::pair<int, int>> edges(N - 1);
    for (auto &[u, v] : edges) std::cin >> u >> v;
    kotone::heavy_light_decomposition_tree tree(edges);

    std::vector<affine> vec(N);
    for (int i = 0; i < N; i++) vec[tree.order(i)] = init[i];
    atcoder::segtree<affine, op, e> seg(vec);
    atcoder::segtree<affine, op_rev, e> segrev(vec);

    while (Q--) {
        int t, u, v, x;
        std::cin >> t >> u >> v >> x;
        if (t == 0) {
            seg.set(tree.order(u), {v, x});
            segrev.set(tree.order(u), {v, x});
            continue;
        }
        affine prod = e();
        for (auto [s, t] : tree.aggregate(u, v)) {
            int l = tree.order(s), r = tree.order(t);
            if (l <= r) prod = op(prod, seg.prod(l, r + 1));
            else prod = op(prod, segrev.prod(r, l + 1));
        }
        mint result = prod.first * x + prod.second;
        std::cout << result.val() << '\n';
    }
}
