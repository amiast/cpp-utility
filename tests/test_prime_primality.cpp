// Verified with: https://judge.yosupo.jp/problem/primality_test
// Details: https://judge.yosupo.jp/submission/413359

#include <iostream>
#include <kotone/prime>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        int64_t N;
        std::cin >> N;
        if (kotone::is_prime(N)) std::cout << "Yes\n";
        else std::cout << "No\n";
    }
}
