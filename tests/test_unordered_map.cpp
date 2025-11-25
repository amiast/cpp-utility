// Verified with: https://judge.yosupo.jp/problem/associative_array
// Details: https://judge.yosupo.jp/submission/331681

#include <iostream>
#include <kotone/unordered_map>

int main() {
    int Q;
    std::cin >> Q;
    kotone::unordered_map<int64_t, int64_t> map;
    while (Q--) {
        int t;
        int64_t k, v;
        std::cin >> t >> k;
        if (t == 0) std::cin >> v, map[k] = v;
        else std::cout << map[k] << std::endl;
    }
}
