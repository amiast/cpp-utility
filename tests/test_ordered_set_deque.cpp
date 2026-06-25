// Verified with: https://judge.yosupo.jp/problem/deque
// Details: https://judge.yosupo.jp/submission/317633

#include <iostream>
#include <utility>
#include <kotone/ordered_set>

int main() {
    int Q;
    std::cin >> Q;
    kotone::ordered_set<std::pair<int, int>> deque;
    for (int i = 0, t, x; i < Q; i++) {
        std::cin >> t;
        if (t == 0) {
            std::cin >> x;
            deque.emplace(-i, x);
        }
        else if (t == 1) {
            std::cin >> x;
            deque.emplace(i, x);
        }
        else if (t == 2) deque.erase(*deque.begin());
        else if (t == 3) deque.erase(*deque.rbegin());
        else {
            std::cin >> x;
            std::cout << deque.get_nth(x)->second << std::endl;
        }
    }
}
