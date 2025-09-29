// Verified with: https://judge.yosupo.jp/problem/predecessor_problem
// Details: https://judge.yosupo.jp/submission/317604

#include <iostream>
#include <vector>
#include <kotone/ordered_set>

int main() {
    int N, Q;
    std::string T;
    std::cin >> N >> Q >> T;
    std::vector<int> vec;
    for (int i = 0; i < static_cast<int>(T.size()); i++) {
        if (T[i] == '1') vec.push_back(i);
    }
    kotone::ordered_set<int> set(vec);
    while (Q--) {
        int c, k;
        std::cin >> c >> k;
        if (c == 0) {
            bool has_elem = set.contains(k);
            auto [iter, inserted] = set.insert(k);
            assert(has_elem == !inserted);
            assert(*iter == k);
            assert(set.contains(k));
        }
        else if (c == 1) {
            bool has_elem = set.contains(k);
            bool erased = set.erase(k);
            assert(has_elem == erased);
            assert(!set.contains(k));
        }
        else if (c == 2) std::cout << set.contains(k) << std::endl;
        else if (c == 3) {
            auto iter = set.lower_bound(k);
            if (iter == set.end()) std::cout << -1 << std::endl;
            else std::cout << *iter << std::endl;
        }
        else {
            auto iter = set.upper_bound(k);
            if (iter == set.begin()) std::cout << -1 << std::endl;
            else std::cout << *std::prev(iter) << std::endl;
        }
    }
}
