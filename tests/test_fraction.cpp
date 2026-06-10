// Verified with: https://judge.yosupo.jp/problem/stern_brocot_tree
// Details: https://judge.yosupo.jp/submission/378581

#include <iostream>
#include <vector>
#include <kotone/fraction>

using fraction = kotone::fraction<int>;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        std::string op;
        std::cin >> op;
        if (op == "ENCODE_PATH") {
            int a, b;
            std::cin >> a >> b;
            std::vector<int> rle = fraction(a, b).rle();
            std::cout << rle.size();
            for (int d : rle) {
                if (d < 0) std::cout << " L " << -d;
                else std::cout << " R " << d;
            }
            std::cout << '\n';
        }
        if (op == "DECODE_PATH") {
            int k;
            std::cin >> k;
            std::vector<int> rle(k);
            for (int i = 0; i < k; i++) {
                char c;
                int d;
                std::cin >> c >> d;
                if (c == 'L') rle[i] = -d;
                else rle[i] = d;
            }
            auto [p, q] = fraction(rle).val();
            std::cout << p << ' ' << q << '\n';
        }
        if (op == "LCA") {
            int a, b, c, d;
            std::cin >> a >> b >> c >> d;
            fraction f(a, b), g(c, d);
            auto [p, q] = f.lca(g).val();
            std::cout << p << ' ' << q << '\n';
        }
        if (op == "ANCESTOR") {
            int k, a, b;
            std::cin >> k >> a >> b;
            fraction f(a, b);
            if (k > f.depth()) std::cout << "-1\n";
            else {
                f.ascend(f.depth() - k);
                auto [p, q] = f.val();
                std::cout << p << ' ' << q << '\n';
            }
        }
        if (op == "RANGE") {
            int a, b;
            std::cin >> a >> b;
            fraction f(a, b);
            auto [p, q] = f.lower_bound();
            auto [r, s] = f.upper_bound();
            std::cout << p << ' ' << q << ' ' << r << ' ' << s << '\n';
        }
    }
}
