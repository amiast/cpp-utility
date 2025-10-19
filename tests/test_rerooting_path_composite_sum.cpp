// Verified with: https://judge.yosupo.jp/problem/tree_path_composite_sum
// Details: https://judge.yosupo.jp/submission/321933

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <atcoder/modint>
#include <kotone/rerooting>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;

S merge(S l, S r) {
    return {l.first + r.first, l.second + r.second};
}

std::vector<int> A;
std::map<std::pair<int, int>, std::pair<int, int>> map;
S apply(S dp, int child, int parent) {
    auto [sum, size] = dp;
    auto [b, c] = map[{child, parent}];
    return {b * (sum + A[child]) + c * (size + 1), size + 1};
}

S identity() {
    return {0, 0};
}

int main() {
    int N;
    std::cin >> N;
    A.resize(N);
    for (int &a : A) std::cin >> a;
    kotone::rerooting<S, merge, apply, identity> tree(N);
    for (int i = 1; i < N; i++) {
        int u, v, b, c;
        std::cin >> u >> v >> b >> c;
        tree.add_edge(u, v);
        map[{u, v}] = map[{v, u}] = {b, c};
    }
    std::vector<S> result = tree.evaluate();
    for (int i = 0; i < N; i++) {
        std::cout << (result[i].first + A[i]).val() << std::endl;
    }
}
