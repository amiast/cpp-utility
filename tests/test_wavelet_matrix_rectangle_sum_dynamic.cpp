// Verified with: https://judge.yosupo.jp/problem/point_add_rectangle_sum
// Details: https://judge.yosupo.jp/submission/366259

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
    std::vector<std::tuple<int, int, int, int>> points(N);
    for (auto &[x, y, w, i] : points) std::cin >> x >> y >> w, i = -1;
    std::vector<std::tuple<int, int, int, int, int>> queries(Q);
    for (int i = 0; i < Q; i++) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int x, y, w;
            std::cin >> x >> y >> w;
            points.emplace_back(x, y, w, i);
            queries[i] = {t, x, y, w, -1};
        } else {
            int l, d, r, u;
            std::cin >> l >> d >> r >> u;
            queries[i] = {t, l, d, r, u};
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

    for (auto [t, l, d, r, u] : queries) {
        if (t == 0) {
            wm.set(u, r);
            continue;
        }
        l = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), l));
        r = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), r));
        std::cout << wm.prod(l, r, u) - wm.prod(l, r, d) << std::endl;
    }
}
