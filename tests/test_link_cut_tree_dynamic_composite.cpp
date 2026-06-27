// Verified with: https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
// Details: https://judge.yosupo.jp/submission/382546

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/link_cut_tree>

using mint = atcoder::modint998244353;
std::vector<std::pair<mint, mint>> affine, acc, rev;
void on_update(int p, int l, int r) {
    acc[p] = rev[p] = affine[p];
    if (l != -1) acc[p] = {acc[p].first * acc[l].first, acc[p].first * acc[l].second + acc[p].second};
    if (r != -1) acc[p] = {acc[r].first * acc[p].first, acc[r].first * acc[p].second + acc[r].second};
    if (r != -1) rev[p] = {rev[p].first * rev[r].first, rev[p].first * rev[r].second + rev[p].second};
    if (l != -1) rev[p] = {rev[l].first * rev[p].first, rev[l].first * rev[p].second + rev[l].second};
}
void on_reverse(int p, int, int) {
    std::swap(acc[p], rev[p]);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    affine.resize(N);
    acc.resize(N);
    rev.resize(N);
    for (int i = 0; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        affine[i] = acc[i] = rev[i] = {a, b};
    }
    kotone::link_cut_tree<on_update, on_reverse> lct(N);
    for (int i = 1; i < N; i++) {
        int u, v;
        std::cin >> u >> v;
        lct.reorient(u);
        lct.link(u, v);
    }
    while (Q--) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int u, v, w, x;
            std::cin >> u >> v >> w >> x;
            lct.reorient(u);
            lct.cut(v);
            lct.reorient(w);
            lct.link(w, x);
        } else if (t == 1) {
            int p, c, d;
            std::cin >> p >> c >> d;
            lct.access(p);
            affine[p] = {c, d};
            lct.update(p);
        } else {
            int u, v, x;
            std::cin >> u >> v >> x;
            lct.reorient(u);
            lct.access(v);
            mint result = acc[v].first * x + acc[v].second;
            std::cout << result.val() << '\n';
        }
    }
}
