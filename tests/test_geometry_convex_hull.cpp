// Verified with: https://judge.yosupo.jp/problem/static_convex_hull
// Details: https://judge.yosupo.jp/submission/314072

#include <iostream>
#include <vector>
#include <algorithm>
#include <kotone/geometry>

int main() {
    int T;
    std::cin >> T;
    while (T--) {
        int N;
        std::cin >> N;
        std::vector<kotone::point<int64_t>> points;
        while (N--) {
            int x, y;
            std::cin >> x >> y;
            points.emplace_back(x, y);
        }
        std::ranges::sort(points);
        std::vector<kotone::point<int64_t>> hull = kotone::convex_hull(points);
        std::cout << hull.size() << std::endl;
        for (kotone::point<int64_t> p : hull) std::cout << p.x() << ' ' << p.y() << std::endl;
    }
}
