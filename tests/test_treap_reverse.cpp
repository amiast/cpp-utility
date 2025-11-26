// Verified with: https://judge.yosupo.jp/problem/range_reverse_range_sum
// Details: https://judge.yosupo.jp/submission/331706

#include <iostream>
#include <vector>
#include <kotone/memory_pool>
#include <kotone/treap>

int64_t op(int64_t a, int64_t b) { return a + b; }
int64_t e() { return 0; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int64_t> A(N);
    for (int64_t &a : A) std::cin >> a;
    kotone::treap_manager<int64_t, op, e> treap;
    auto *root = treap.allocate_treap(A);
    while (Q--) {
        int t, l, r;
        std::cin >> t >> l >> r;
        if (t == 0) treap.reverse(root, l, r);
        else std::cout << treap.get_prod(root, l, r) << std::endl;
    }
}
