// Verified with: https://judge.yosupo.jp/problem/range_kth_smallest
// Details: https://judge.yosupo.jp/submission/365679

#include <iostream>
#include <vector>
#include <kotone/wavelet_matrix>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;

    kotone::wavelet_matrix<30> wm(A);
    while (Q--) {
        int l, r, k;
        std::cin >> l >> r >> k;
        std::cout << wm.nth_smallest(l, r, k) << std::endl;
    }
}
