// Verified with: https://judge.yosupo.jp/problem/primality_test
// Details: https://judge.yosupo.jp/submission/366196

#include <iostream>
#include <kotone/prime>

int main() {
    int T;
    std::cin >> T;
    while (T--) {
        int64_t N;
        std::cin >> N;
        if (kotone::is_prime(N)) std::cout << "Yes" << std::endl;
        else std::cout << "No" << std::endl;
    }
}
