// Verified with: https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
// Details: https://judge.yosupo.jp/submission/331728

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/lazy_treap>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
S e() { return {0, 0}; }
using F = std::pair<mint, mint>;
S mapping(F f, S x) { return {f.first * x.first + f.second * x.second, x.second}; }
F composition(F f, F g) { return {f.first * g.first, f.first * g.second + f.second}; }
F id() { return {1, 0}; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<S> A(N);
    for (int i = 0; i < N; i++) {
        int a;
        std::cin >> a;
        A[i].first = a;
        A[i].second = 1;
    }
    kotone::lazy_treap_manager<S, op, e, F, mapping, composition, id> treap;
    auto root = treap.allocate_treap(A);
    while (Q--) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int i, x;
            std::cin >> i >> x;
            treap.insert(root, treap.allocate_node({x, 1}), i);
        }
        else if (t == 1) {
            int i;
            std::cin >> i;
            treap.deallocate_treap(treap.remove(root, i));
        }
        else if (t == 2) {
            int l, r;
            std::cin >> l >> r;
            treap.reverse(root, l, r);
        }
        else if (t == 3) {
            int l, r, b, c;
            std::cin >> l >> r >> b >> c;
            treap.apply(root, {b, c}, l, r);
        }
        else {
            int l, r;
            std::cin >> l >> r;
            std::cout << treap.get_prod(root, l, r).first.val() << std::endl;
        }
    }
}
