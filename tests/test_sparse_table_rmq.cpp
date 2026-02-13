// Verified with: https://judge.yosupo.jp/problem/staticrmq
// Details: https://judge.yosupo.jp/submission/352456

#include <iostream>
#include <vector>
#include <kotone/sparse_table>

int op(int a, int b) { return a < b ? a : b; }
int e() { return 1 << 30; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;
    kotone::disjoint_sparse_table<int, op, e> st(A);
    while (Q--) {
        int l, r;
        std::cin >> l >> r;
        std::cout << st.prod(l, r) << std::endl;
    }
}
