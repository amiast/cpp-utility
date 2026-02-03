// Verified with: https://judge.yosupo.jp/problem/vertex_set_path_composite
// Details: https://judge.yosupo.jp/submission/349942

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <atcoder/segtree>

using mint = atcoder::modint998244353;
using affine = std::pair<mint, mint>;
affine op(affine p, affine u) { return {u.first * p.first, u.first * p.second + u.second}; }
affine op_rev(affine p, affine u) { return op(u, p); }
affine e() { return {1, 0}; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<affine> init(N);
    for (int i = 0; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        init[i].first = a, init[i].second = b;
    }

    std::vector<std::vector<int>> tree(N);
    for (int i = 1; i < N; i++) {
        int u, v;
        std::cin >> u >> v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    std::vector<int> size(N), parent(N);
    auto eval_size = [&](auto &eval_size, int u, int p) -> void {
        size[u] = 1;
        parent[u] = p;
        for (int &v : tree[u]) {
            if (v == p) continue;
            eval_size(eval_size, v, u);
            size[u] += size[v];
            if (size[v] > size[tree[u][0]]) std::swap(v, tree[u][0]);
        }
    };
    eval_size(eval_size, 0, 0);

    int id = 0;
    std::vector<int> order(N), head(N);
    auto eval_order = [&](auto &eval_order, int u, int p) -> void {
        order[u] = id++;
        for (int v : tree[u]) {
            if (v == p) continue;
            head[v] = v == tree[u][0] ? head[u] : v;
            eval_order(eval_order, v, u);
        }
    };
    eval_order(eval_order, 0, 0);

    std::vector<affine> vec(N);
    for (int i = 0; i < N; i++) vec[order[i]] = init[i];
    atcoder::segtree<affine, op, e> seg(vec);
    atcoder::segtree<affine, op_rev, e> segrev(vec);

    while (Q--) {
        int t, u, v, x;
        std::cin >> t >> u >> v >> x;
        if (t == 0) {
            seg.set(order[u], {v, x});
            segrev.set(order[u], {v, x});
            continue;
        }
        affine pfx = e(), sfx = e();
        while (head[u] != head[v]) {
            if (order[u] < order[v]) {
                sfx = op(seg.prod(order[head[v]], order[v] + 1), sfx);
                v = parent[head[v]];
            } else {
                pfx = op(pfx, segrev.prod(order[head[u]], order[u] + 1));
                u = parent[head[u]];
            }
        }
        affine mid;
        if (order[u] <= order[v]) mid = seg.prod(order[u], order[v] + 1);
        else mid = segrev.prod(order[v], order[u] + 1);
        affine composition = op(pfx, op(mid, sfx));
        mint result = composition.first * x + composition.second;
        std::cout << result.val() << std::endl;
    }
}
