// Verified with: https://judge.yosupo.jp/problem/point_set_range_composite
// Details: https://judge.yosupo.jp/submission/331705

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/memory_pool>
#include <kotone/treap>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S a, S b) { return {a.first * b.first, a.second * b.first + b.second}; }
S e() { return {1, 0}; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<S> vec(N);
    for (int i = 0; i < N; i++) {
        int a, b;
        std::cin >> a >> b;
        vec[i] = {a, b};
    }
    kotone::treap_manager<S, op, e> treap;
    auto root = treap.allocate_treap(vec);
    while (Q--) {
        int t, l, r, x;
        std::cin >> t >> l >> r >> x;
        if (t == 0) treap.set_val(root, l, {r, x});
        else {
            auto [a, b] = treap.get_prod(root, l, r);
            mint result = a * x + b;
            std::cout << result.val() << std::endl;
        }
    }
}
