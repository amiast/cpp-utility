#ifndef KOTONE_LINEAR_MATROID_PARITY
#define KOTONE_LINEAR_MATROID_PARITY 1

#include <vector>
#include <cassert>
#include <kotone/matrix>
#include <kotone/random>
#include <kotone/internal_type_traits>

namespace kotone {

// Given pairs of indices of the specified vectors,
// returns a maximum pairing such that the collection of the vectors is linearly independent.
// Requires `vecs` to contain vectors of identical dimensions.
// Requires `0 <= a, b < vecs.size()` for each `{a, b}` in `pairs`.
// Reference: https://hitonanode.github.io/cplib-cpp/combinatorial_opt/linear_matroid_parity.hpp.html
template <compatible_modint mint> std::vector<bool> maximum_pairing(
    std::vector<std::vector<mint>> vecs,
    std::vector<std::pair<int, int>> pairs
) {
    int n = vecs.size(), p = pairs.size();
    for (int i = 0; i < n; i++) assert(vecs[i].size() == vecs[0].size());
    for (auto [a, b] : pairs) {
        assert(0 <= a && a < n);
        assert(0 <= b && b < n);
    }
    if (pairs.empty()) return {};
    if (vecs[0].size() & 1) for (int i = 0; i < n; i++) vecs[i].emplace_back();
    int m = vecs[0].size(), P = pairs.size();
    std::vector mat(m, std::vector<mint>(m));
    std::vector<mint> xs(P);
    while (true) {
        for (int k = 0; k < P; k++) {
            auto [a, b] = pairs[k];
            xs[k] = randint();
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < m; j++) {
                    mat[i][j] += xs[k] * (vecs[a][i] * vecs[b][j] - vecs[a][j] * vecs[b][i]);
                }
            }
        }
        int r = matrix::basis(mat).size();
        if (r == m) break;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) mat[i][j] = 0;
        }
        for (int t = 0; t * 2 + r < m; t++) {
            vecs.emplace_back(m);
            for (mint &v : vecs.back()) v = randint();
            vecs.emplace_back(m);
            for (mint &v : vecs.back()) v = randint();
            pairs.emplace_back(n, n + 1);
            xs.emplace_back();
            n += 2;
            P++;
        }
    }
    mat = matrix::inv(mat).first;
    std::vector<bool> result(P, true);
    for (int k = P - 1; k >= 0; k--) {
        auto [a, b] = pairs[k];
        std::vector<mint> acc_a(m), acc_b(m);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                acc_a[i] += mat[i][j] * vecs[a][j];
                acc_b[i] += mat[i][j] * vecs[b][j];
            }
        }
        mint acc = 0;
        for (int i = 0; i < m; i++) acc += vecs[a][i] * acc_b[i];
        acc = acc * xs[k] + 1;
        if (acc == 0) continue;
        result[k] = false;
        acc = xs[k] / acc;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                mat[i][j] -= acc * (acc_a[i] * acc_b[j] - acc_a[j] * acc_b[i]);
            }
        }
    }
    result.resize(p);
    return result;
}

}  // namespace kotone

#endif  // KOTONE_LINEAR_MATROID_PARITY
