// Verified with: https://judge.yosupo.jp/problem/jump_on_tree
// Details: https://judge.yosupo.jp/submission/382509

#include <iostream>
#include <kotone/link_cut_tree>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    kotone::link_cut_tree lct(N);
    for (int i = 1; i < N; i++) {
        int u, v;
        std::cin >> u >> v;
        lct.reorient(u);
        lct.link(u, v);
    }
    while (Q--) {
        int s, t, i;
        std::cin >> s >> t >> i;
        lct.reorient(s);
        std::cout << lct.level_ancestor(t, i) << '\n';
    }
}
