// Verified with: https://judge.yosupo.jp/problem/eertree
// Details: https://judge.yosupo.jp/submission/393177

#include <iostream>
#include <vector>
#include <kotone/palindrome_tree>

std::vector<int> result;
void on_push(int r, int l) {
    result.push_back(l);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string S;
    std::cin >> S;
    int N = S.size();
    std::vector<int> T(N);
    for (int i = 0; i < N; i++) T[i] = S[i] - 'a';
    result.reserve(N);
    auto tree = kotone::palindrome_tree<26, nullptr, on_push>(T);
    int n = tree.size();
    std::cout << n - 2 << '\n';
    for (int i = 2; i < n; i++) {
        std::cout << tree[i].parent - 1 << ' ' << tree[i].link - 1 << '\n';
    }
    for (int r : result) std::cout << r - 1 << ' ';
    std::cout << '\n';
}
