#ifndef KOTONE_CHROMATIC_NUMBER_HPP
#define KOTONE_CHROMATIC_NUMBER_HPP 1

#include <vector>
#include <bit>
#include <cassert>
#include <kotone/mod_math>

namespace kotone {

// Returns the chromatic number of the specified graph.
// Requires `graph` to be a valid adjacency matrix of a simple, undirected graph.
// Requires `0 <= graph.size() <= 30`.
// Requires `graph[i].size() == graph.size()` for `0 <= i < graph.size()`.
// Requires compiler-provided type `__int128`.
int chromatic_number(const std::vector<std::vector<bool>> &graph) {
    int n = graph.size();
    assert(0 <= n && n <= 30);
    for (int i = 0; i < n; i++) {
        assert(int(graph[i].size()) == n);
        assert(!graph[i][i]);
        for (int j = 0; j < i; j++) {
            assert(graph[i][j] == graph[j][i]);
        }
    }
    std::vector<unsigned> adj(n);
    for (int i = 0; i < n; i++) {
        assert(!graph[i][i]);
        for (int j = 0; j < n; j++) {
            adj[i] |= graph[i][j] << j;
        }
    }
    unsigned m = 1 << n;
    std::vector<int> dp(m);
    dp[0] = 1;
    for (unsigned k = 1; k < m; k++) {
        int i = std::bit_width(k & -k) - 1;
        unsigned nk = k ^ 1 << i;
        if (dp[nk] && !(adj[i] & nk)) dp[k] = 1;
    }
    for (int i = 0; i < n; i++) {
        for (unsigned k = 0; k < m; k++) {
            if (k >> i & 1) dp[k] += dp[k ^ 1 << i];
        }
    }
    uint64_t MOD = (1ULL << 61) - 1;
    for (int i = 0; i < n; i++) {
        uint64_t acc = 0;
        for (unsigned k = 0; k < m; k++) {
            if ((n - std::popcount(k)) % 2 == 0) {
                acc = sum_mod(acc, pow_mod(dp[k], i, MOD), MOD);
            } else {
                acc = sum_mod(acc, prod_mod(pow_mod(dp[k], i, MOD), -1, MOD), MOD);
            }
        }
        if (acc) return i;
    }
    return n;
}

}  // namespace kotone

#endif  // KOTONE_CHROMATIC_NUMBER_HPP
