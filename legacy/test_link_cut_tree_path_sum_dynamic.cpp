// Verified with: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
// Details: https://judge.yosupo.jp/submission/327616

#include <iostream>
#include <vector>
#include <kotone/link_cut_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::extended_link_cut_tree<int64_t> tree(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        tree.set_weight(i, a);
    }
    for (int i = 1; i < N; i++) {
        int u, v;
        std::cin >> u >> v;
        tree.link(u, v);
    }
    while (Q--) {
        int t, u, v;
        std::cin >> t >> u >> v;
        if (t == 0) {
            int w, x;
            std::cin >> w >> x;
            tree.cut(u, v);
            tree.link(w, x);
        } else if (t == 1) {
            tree.set_weight(u, tree.get_weight(u) + v);
        } else {
            std::cout << tree.get_path_sum(u, v) << std::endl;
        }
    }
}
