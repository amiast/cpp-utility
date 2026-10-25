// Verified with: https://judge.yosupo.jp/problem/prefix_sum_of_binomial_coefficient
// Details: https://judge.yosupo.jp/submission/413355

#include <iostream>
#include <vector>
#include <atcoder/modint>
#include <kotone/modint_utility>
#include <kotone/mo_algorithm>

using mint = atcoder::modint998244353;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    std::vector<std::pair<int, int>> queries(T);
    for (auto &[k, n] : queries) std::cin >> n >> k;
    kotone::modint_utility<mint> util(200000);
    mint acc = 1;
    std::vector<mint> result(T);
    auto solve = [&](int t) { result[t] = acc; };
    auto incr_l = [&](int l, int r) { acc += util.comb(r, l + 1); };
    auto decr_l = [&](int l, int r) { acc -= util.comb(r, l + 1); };
    auto incr_r = [&](int l, int r) { acc *= 2; acc -= util.comb(r, l); };
    auto decr_r = [&](int l, int r) { acc += util.comb(r, l); acc *= util.inv(2); };
    kotone::eval_range_queries(queries, kotone::zigzag_index<200001, 447>, incr_l, decr_l, incr_r, decr_r, solve);
    for (mint &r : result) std::cout << r.val() << '\n';
}
