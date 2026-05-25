// Verified with: https://judge.yosupo.jp/problem/chromatic_number
// Details: https://judge.yosupo.jp/submission/374816

#include <iostream>
#include <vector>
#include <kotone/chromatic_number>

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector graph(N, std::vector<bool>(N));
    while (M--) {
        int u, v;
        std::cin >> u >> v;
        graph[u][v] = graph[v][u] = true;
    }
    std::cout << kotone::chromatic_number(graph) << std::endl;
}
