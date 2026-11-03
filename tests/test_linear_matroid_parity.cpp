// Verified with: https://judge.yosupo.jp/problem/general_matching
// Details: https://judge.yosupo.jp/submission/414974 (TLE)

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/linear_matroid_parity>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, M;
    std::cin >> N >> M;
    std::vector<std::pair<int, int>> edges(M);
    for (auto &[u, v] : edges) std::cin >> u >> v;
    std::vector<bool> matching = kotone::maximum_pairing(kotone::matrix::identity<mint>(N), edges);
    std::vector<int> result;
    for (int i = 0; i < M; i++) if (matching[i]) result.push_back(i);
    std::cout << result.size() << '\n';
    for (int i : result) std::cout << edges[i].first << ' ' << edges[i].second << '\n';
}
