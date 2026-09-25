// Verified with: https://judge.yosupo.jp/problem/persistent_range_affine_range_sum
// Details: https://judge.yosupo.jp/submission/405959 (MLE)

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/rbst>

using mint = atcoder::modint998244353;
using S = std::pair<mint, int>;
S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
S e() { return {0, 0}; }
using F = std::pair<mint, mint>;
S mapping(F f, S x) { return {f.first * x.first + f.second * x.second, x.second}; }
F composition(F f, F g) { return {f.first * g.first, f.first * g.second + f.second}; }
F id() { return {1, 0}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<S> vec(N);
    for (auto &[x, c] : vec) {
        int a;
        std::cin >> a;
        x = a;
        c = 1;
    }

    std::vector<int> roots(Q + 1);
    kotone::randomized_binary_search_tree<S, op, e, F, mapping, composition, id, true> rbst(roots[0], vec);

    for (int i = 1; i <= Q; i++) {
        int t, k;
        std::cin >> t >> k;
        k++;
        if (t == 0) {
            int l, r, b, c;
            std::cin >> l >> r >> b >> c;
            int L, M, R;
            std::tie(M, R) = rbst.split(roots[k], r);
            std::tie(L, M) = rbst.split(M, l);
            M = rbst.apply(M, {b, c});
            roots[i] = rbst.merge(rbst.merge(L, M), R);
        } else if (t == 1) {
            int s, l, r;
            std::cin >> s >> l >> r;
            s++;
            int L, M, R;
            std::tie(M, R) = rbst.split(roots[k], r);
            std::tie(L, M) = rbst.split(M, l);
            int S = rbst.split(rbst.split(roots[s], r).first, l).second;
            roots[i] = rbst.merge(rbst.merge(L, S), R);
        } else {
            int l, r;
            std::cin >> l >> r;
            int M = rbst.split(rbst.split(roots[k], r).first, l).second;
            std::cout << rbst.prod(M).first.val() << '\n';
        }
    }
}
