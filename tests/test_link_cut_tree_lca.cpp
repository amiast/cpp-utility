// Verified with: https://judge.yosupo.jp/problem/lca
// Details: https://judge.yosupo.jp/submission/319074

#include <iostream>
#include <vector>
#include <kotone/link_cut_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::extended_link_cut_tree tree(N);
    for (int i = 1; i < N; i++) {
        int p;
        std::cin >> p;
        tree.link(i, p);
    }
    while (Q--) {
        int u, v;
        std::cin >> u >> v;
        std::cout << tree.get_lca(u, v) << std::endl;
    }
}
