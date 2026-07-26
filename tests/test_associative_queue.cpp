// Verified with: https://judge.yosupo.jp/problem/queue_operate_all_composite
// Details: https://judge.yosupo.jp/submission/388273

#include <iostream>
#include <atcoder/modint>
#include <kotone/associative_queue>

using mint = atcoder::modint998244353;
using S = std::pair<mint, mint>;
S op(S l, S r) { return {l.first * r.first, l.second * r.first + r.second}; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int Q;
    std::cin >> Q;
    kotone::associative_queue<S, op> queue;
    while (Q--) {
        int t;
        std::cin >> t;
        if (t == 0) {
            int a, b;
            std::cin >> a >> b;
            queue.push({a, b});
        } else if (t == 1) {
            queue.pop();
        } else {
            int x;
            std::cin >> x;
            if (queue.empty()) std::cout << x << '\n';
            else {
                auto [a, b] = queue.query();
                std::cout << (a * x + b).val() << '\n';
            }
        }
    }
}
