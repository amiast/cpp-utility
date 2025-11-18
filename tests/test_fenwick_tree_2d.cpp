// Verified with: https://judge.yosupo.jp/problem/point_add_rectangle_sum
// Details: https://judge.yosupo.jp/submission/329767

#include <iostream>
#include <vector>
#include <array>
#include <kotone/coord_compress>
#include <kotone/fenwick_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    kotone::coord_compress_hashmap<int> xmap, ymap;
    std::vector<std::array<int, 3>> init(N);
    for (auto &[x, y, w] : init) {
        std::cin >> x >> y >> w;
        xmap.insert(x);
        ymap.insert(y);
    }
    std::vector<std::array<int, 5>> queries(Q);
    for (auto &[t, l, d, r, u] : queries) {
        std::cin >> t >> l >> d >> r;
        xmap.insert(l);
        ymap.insert(d);
        if (t == 1) {
            std::cin >> u;
            xmap.insert(r);
            ymap.insert(u);
        }
    }

    int H = xmap.size(), W = ymap.size();
    kotone::fenwick_tree_2d<int64_t> bit(H, W);
    for (auto [x, y, w] : init) bit.add(xmap[x], ymap[y], w);
    for (auto [t, l, d, r, u] : queries) {
        if (t == 0) bit.add(xmap[l], ymap[d], r);
        else std::cout << bit.sum(xmap[l], ymap[d], xmap[r], ymap[u]) << std::endl;
    }
}
