// Verified with: https://judge.yosupo.jp/problem/min_cost_b_flow
// Details: https://judge.yosupo.jp/submission/400846 (TLE)

#include <iostream>
#include <vector>
#include <kotone/minimum_cost_flow>

std::ostream& operator<<(std::ostream &out, __int128_t x) {
    if (x == 0) {
        out << 0;
        return out;
    }
    if (x < 0) out << '-', x = -x;
    std::string s;
    while (x) {
        s += '0' + x % 10;
        x /= 10;
    }
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, M;
    std::cin >> N >> M;
    kotone::mincost_network_graph<__int128_t, __int128_t> graph(N);
    for (int i = 0; i < N; i++) {
        int b;
        std::cin >> b;
        graph.add_balance(i, b);
    }
    for (int i = 0; i < M; i++) {
        int s, t, l, u, c;
        std::cin >> s >> t >> l >> u >> c;
        graph.add_edge(s, t, l, u, c);
    }
    auto result = graph.flow_ssp();
    if (!result.feasible) std::cout << "infeasible\n";
    else {
        std::cout << result.cost << '\n';
        for (int i = 0; i < N; i++) std::cout << result.potential[i] << '\n';
        for (int i = 0; i < M; i++) std::cout << result.flow[i] << '\n';
    }
}
