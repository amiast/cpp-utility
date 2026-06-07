// Verified with: https://judge.yosupo.jp/problem/sort_points_by_argument
// Details: https://judge.yosupo.jp/submission/377765

#include <iostream>
#include <vector>
#include <algorithm>
#include <kotone/geometry>

using point = kotone::point<int64_t>;

int main() {
    int N;
    std::cin >> N;
    std::vector<point> coords;
    int num_origin = 0;
    while (N--) {
        int x, y;
        std::cin >> x >> y;
        if (x == 0 && y == 0) num_origin++;
        else coords.emplace_back(-x, y);
    }
    std::sort(coords.rbegin(), coords.rend(), point::arg_less{});
    for (auto &p : coords) {
        auto [x, y] = p.get();
        while ((y > 0 || y == 0 && x < 0) && num_origin-- > 0) {
            std::cout << "0 0" << std::endl;
        }
        std::cout << -x << ' ' << y << std::endl;
    }
    while (num_origin-- > 0) std::cout << "0 0" << std::endl;
}
