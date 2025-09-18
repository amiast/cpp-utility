// Verified with: https://judge.yosupo.jp/problem/unionfind_with_potential
// Details: https://judge.yosupo.jp/submission/315363

#include <iostream>
#include <atcoder/modint>
#include <kotone/dsu>

using mint = atcoder::modint998244353;

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::dsu<mint> dsu(N);
    while (Q--) {
        int t, u, v;
        std::cin >> t >> u >> v;
        if (t == 0) {
            int x;
            std::cin >> x;
            if (dsu.connected(u, v) && dsu.potential_diff(u, v) != x) std::cout << 0 << std::endl;
            else std::cout << 1 << std::endl;
            dsu.merge(u, v, x);
        } else {
            if (dsu.connected(u, v)) std::cout << dsu.potential_diff(u, v).val() << std::endl;
            else std::cout << -1 << std::endl;
        }
    }
}
