// Verified with: https://judge.yosupo.jp/problem/static_range_sum
// Details: https://judge.yosupo.jp/submission/352455

#include <iostream>
#include <vector>
#include <kotone/sparse_table>

int64_t op(int64_t a, int64_t b) { return a + b; }
int64_t e() { return 0; }

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int64_t> A(N);
    for (int64_t &a : A) std::cin >> a;
    kotone::disjoint_sparse_table<int64_t, op, e> st(A);
    while (Q--) {
        int l, r;
        std::cin >> l >> r;
        std::cout << st.prod(l, r) << std::endl;
    }
}
