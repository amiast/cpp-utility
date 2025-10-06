// Verified with: https://judge.yosupo.jp/problem/point_set_range_frequency
// Details: https://judge.yosupo.jp/submission/318887

#include <iostream>
#include <vector>
#include <array>
#include <kotone/ordered_set>
#include <kotone/coord_compress>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    kotone::coord_compress_hashmap<int> map;
    for (int &a : A) std::cin >> a, map.insert(a);
    std::vector<std::array<int, 4>> queries(Q);
    for (auto &[t, l, r, v] : queries) {
        std::cin >> t >> l;
        if (t == 0) std::cin >> v, map.insert(v);
        else std::cin >> r >> v;
    }

    std::vector<kotone::ordered_set<int>> coords(map.size());
    for (int i = 0; i < N; i++) coords[map[A[i]]].insert(i);

    for (auto [t, l, r, v] : queries) {
        if (t == 0) {
            coords[map[A[l]]].erase(l);
            coords[map[v]].insert(l);
            A[l] = v;
        } else if (map[v] == -1) {
            std::cout << 0 << std::endl;
        } else {
            std::cout << coords[map[v]].order_of(r) - coords[map[v]].order_of(l) << std::endl;
        }
    }
}
