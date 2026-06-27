// Verified with: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
// Details: https://judge.yosupo.jp/submission/382512

#include <iostream>
#include <kotone/link_cut_tree>

std::vector<int64_t> weight, acc;
void on_update(int p, int l, int r) {
    acc[p] = weight[p];
    if (l != -1) acc[p] += acc[l];
    if (r != -1) acc[p] += acc[r];
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    weight.resize(N);
    acc.resize(N);
    for (int i = 0; i < N; i++) {
        int64_t a;
        std::cin >> a;
        weight[i] = acc[i] = a;
    }
    kotone::link_cut_tree<on_update> lct(N);
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
            int p;
            int64_t x;
            std::cin >> p >> x;
            lct.access(p);
            weight[p] += x;
            lct.update(p);
        } else {
            int u, v;
            std::cin >> u >> v;
            lct.reorient(u);
            lct.access(v);
            std::cout << acc[v] << '\n';
        }
    }
}
