// Verified with: https://judge.yosupo.jp/problem/nim_product_64
// Details: https://judge.yosupo.jp/submission/377753

#include <iostream>
#include <kotone/nimber>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        kotone::nimber a, b;
        std::cin >> a >> b;
        std::cout << a * b << '\n';
    }
}
