// Verified with: https://judge.yosupo.jp/problem/rectangle_sum
// Details: https://judge.yosupo.jp/submission/366258

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
    std::vector<std::tuple<int, int, int>> points(N);
    for (auto &[x, y, w] : points) std::cin >> x >> y >> w;
    std::sort(points.begin(), points.end());

    std::vector<int> xs(N), vec(N);
    std::vector<int64_t> vals(N);
    for (int i = 0; i < N; i++) {
        auto [x, y, w] = points[i];
        xs[i] = x;
        vec[i] = y;
        vals[i] = w;
    }
    kotone::wavelet_matrix_commutative<int64_t, op, e, 30> wm(vec, vals);

    while (Q--) {
        int l, d, r, u;
        std::cin >> l >> d >> r >> u;
        l = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), l));
        r = std::distance(xs.begin(), std::lower_bound(xs.begin(), xs.end(), r));
        std::cout << wm.prod(l, r, u) - wm.prod(l, r, d) << std::endl;
    }
}
