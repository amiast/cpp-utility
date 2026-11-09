// Verified with: https://judge.yosupo.jp/problem/static_range_count_distinct
// Details: https://judge.yosupo.jp/submission/415843

#include <iostream>
#include <vector>
#include <algorithm>
#include <kotone/wavelet_matrix>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;
    std::vector<int> vals = A;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    for (int &a : A) a = std::lower_bound(vals.begin(), vals.end(), a) - vals.begin();
    std::vector<int> prev(N), pos(N);
    for (int i = 0; i < N; i++) prev[i] = pos[A[i]], pos[A[i]] = i + 1;
    kotone::wavelet_matrix<20> wm(prev);
    while (Q--) {
        int l, r;
        std::cin >> l >> r;
        std::cout << wm.freq(l, r, l + 1) << '\n';
    }
}
