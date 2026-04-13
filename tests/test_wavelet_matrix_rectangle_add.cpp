// Verified with: https://judge.yosupo.jp/problem/rectangle_add_point_get
// Details: https://judge.yosupo.jp/submission/366260

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <kotone/wavelet_matrix>

int64_t op(int64_t a, int64_t b) { return a + b; }
int64_t e() { return 0; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<std::tuple<int, int, int, int>> points;
    while (N--) {
        int l, d, r, u, w;
        std::cin >> l >> d >> r >> u >> w;
        points.emplace_back(l, d, w, -1);
        points.emplace_back(l, u, -w, -1);
        points.emplace_back(r, d, -w, -1);
        points.emplace_back(r, u, w, -1);
    }

    std::vector<std::tuple<int, int, int, int, int>> queries;
    while (Q--) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int l, d, r, u, w;
            std::cin >> l >> d >> r >> u >> w;
            points.emplace_back(l, d, 0, queries.size());
            queries.emplace_back(0, l, d, w, -1);
            points.emplace_back(l, u, 0, queries.size());
            queries.emplace_back(0, l, u, -w, -1);
            points.emplace_back(r, d, 0, queries.size());
            queries.emplace_back(0, r, d, -w, -1);
            points.emplace_back(r, u, 0, queries.size());
            queries.emplace_back(0, r, u, w, -1);
        } else {
            int x, y;
            std::cin >> x >> y;
            queries.emplace_back(1, x + 1, y + 1, -1, -1);
        }
    }

    N = points.size();
    std::sort(points.begin(), points.end());
    std::vector<int> xs(N), vec(N);
    std::vector<int64_t> vals(N);
    for (int k = 0; k < N; k++) {
        auto [x, y, w, i] = points[k];
        xs[k] = x;
        vec[k] = y;
        if (i == -1) vals[k] = w;
        else std::get<4>(queries[i]) = k;
    }
    kotone::wavelet_matrix_commutative<int64_t, op, e, 30> wm(vec, vals);

    for (auto [t, x, y, w, k] : queries) {
        if (t == 0) {
            wm.set(k, w);
            continue;
        }
        x = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), x));
        std::cout << wm.prod(0, x, y) << std::endl;
    }
}
