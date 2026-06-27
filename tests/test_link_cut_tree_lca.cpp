// Verified with: https://judge.yosupo.jp/problem/lca
// Details: https://judge.yosupo.jp/submission/382504

#include <iostream>
#include <kotone/link_cut_tree>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    kotone::link_cut_tree lct(N);
    for (int i = 1; i < N; i++) {
        int p;
        std::cin >> p;
        lct.link(i, p);
    }
    while (Q--) {
        int u, v;
        std::cin >> u >> v;
        std::cout << lct.lca(u, v) << '\n';
    }
}
