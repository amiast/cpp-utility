// Verified with: https://judge.yosupo.jp/problem/enumerate_palindromes
// Details: https://judge.yosupo.jp/submission/393429

#include <iostream>
#include <vector>
#include <kotone/manacher>

int main() {
    std::string S;
    std::cin >> S;
    std::vector<int> result = kotone::manacher(S).second;
    for (size_t i = 0; i < result.size(); i++) {
        if (i % 2 == 0) std::cout << (result[i] - 1) / 2 * 2 + 1 << ' ';
        else std::cout << result[i] / 2 * 2 << ' ';
    }
    std::cout << std::endl;
}
