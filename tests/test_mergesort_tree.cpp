// Verified with: https://judge.yosupo.jp/problem/range_kth_smallest
// Details: https://judge.yosupo.jp/submission/315372

#include <iostream>
#include <vector>
#include <kotone/mergesort_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;
    kotone::mergesort_tree mst(A);
    while (Q--) {
        int l, r, k;
        std::cin >> l >> r >> k;
        int low = -1, high = 1000000000;
        while (low + 1 < high) {
            int mid = (low + high) / 2;
            if (mst.count(l, r, mid) <= k) low = mid;
            else high = mid;
        }
        std::cout << high << std::endl;
    }
}
