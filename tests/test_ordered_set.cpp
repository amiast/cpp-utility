// Verified with: https://judge.yosupo.jp/problem/ordered_set
// Details: https://judge.yosupo.jp/submission/317527

#include <iostream>
#include <vector>
#include <kotone/ordered_set>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;
    kotone::ordered_set<int> set(A);
    while (Q--) {
        int t, x;
        std::cin >> t >> x;
        if (t == 0) {
            bool has_elem = set.contains(x);
            auto [iter, inserted] = set.insert(x);
            assert(has_elem == !inserted);
            assert(*iter == x);
            assert(set.contains(x));
        }
        else if (t == 1) {
            bool has_elem = set.contains(x);
            bool erased = set.erase(x);
            assert(has_elem == erased);
            assert(!set.contains(x));
        }
        else if (t == 2) {
            auto iter = set.get_nth(x - 1);
            if (iter == set.end()) std::cout << -1 << std::endl;
            else std::cout << *iter << std::endl;
        }
        else if (t == 3) std::cout << set.order_of(x + 1) << std::endl;
        else if (t == 4) {
            auto iter = set.upper_bound(x);
            if (iter == set.begin()) std::cout << -1 << std::endl;
            else std::cout << *std::prev(iter) << std::endl;
        }
        else {
            auto iter = set.lower_bound(x);
            if (iter == set.end()) std::cout << -1 << std::endl;
            else std::cout << *iter << std::endl;
        }
    }
}
