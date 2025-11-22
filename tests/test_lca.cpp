// Verified with: https://judge.yosupo.jp/problem/lca
// Details: https://judge.yosupo.jp/submission/330693

#include <iostream>
#include <kotone/lca>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::lca_tree tree(N);
    for (int i = 1; i < N; i++) {
        int p;
        std::cin >> p;
        tree.add_edge(p, i);
    }
    while (Q--) {
        int u, v;
        std::cin >> u >> v;
        std::cout << tree.get_lca(u, v) << std::endl;
    }
}
