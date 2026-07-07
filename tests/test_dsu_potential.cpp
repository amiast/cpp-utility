// Verified with: https://judge.yosupo.jp/problem/unionfind_with_potential
// Details: https://judge.yosupo.jp/submission/384381

#include <iostream>
#include <atcoder/modint>
#include <kotone/dsu>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    kotone::dsu<mint> dsu(N);
    while (Q--) {
        int t, u, v;
        std::cin >> t >> u >> v;
        if (t == 0) {
            int x;
            std::cin >> x;
            std::cout << (!dsu.connected(u, v) || dsu.potential_diff(u, v) == x) << '\n';
            dsu.merge(u, v, x);
        } else {
            if (dsu.connected(u, v)) std::cout << dsu.potential_diff(u, v).val() << '\n';
            else std::cout << -1 << '\n';
        }
    }
}
