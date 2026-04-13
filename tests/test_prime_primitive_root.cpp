// Verified with: https://judge.yosupo.jp/problem/primitive_root
// Details: https://judge.yosupo.jp/submission/366198

#include <iostream>
#include <kotone/prime>

int main() {
    int T;
    std::cin >> T;
    while (T--) {
        int64_t p;
        std::cin >> p;
        assert(kotone::is_prime(p));
        std::cout << kotone::least_primitive_root(p) << std::endl;
    }
}
