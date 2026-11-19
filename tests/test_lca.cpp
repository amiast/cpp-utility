// Verified with: https://judge.yosupo.jp/problem/lca
// Details: https://judge.yosupo.jp/submission/417991

#include <iostream>
#include <kotone/lca>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<std::pair<int, int>> edges(N - 1);
    for (int i = 1; i < N; i++) {
        int p;
        std::cin >> p;
        edges[i - 1] = {p, i};
    }
    kotone::lca_tree tree(N, edges);
    while (Q--) {
        int u, v;
        std::cin >> u >> v;
        std::cout << tree.lca(u, v) << '\n';
    }
}
