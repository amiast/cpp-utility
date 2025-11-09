// Verified with: https://judge.yosupo.jp/problem/jump_on_tree
// Details: https://judge.yosupo.jp/submission/327614

#include <iostream>
#include <vector>
#include <kotone/link_cut_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::extended_link_cut_tree<int> tree(N);
    for (int i = 1; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        tree.link(a, b);
    }
    while (Q--) {
        int s, t, i;
        std::cin >> s >> t >> i;
        std::cout << tree.get_nth_node(s, t, i) << std::endl;
    }
}
