// Verified with: https://yukicoder.me/problems/no/399
// Details: https://yukicoder.me/submissions/1169760

#include <iostream>
#include <vector>
#include <kotone/link_cut_tree>

std::vector<int> acc, lazy;
void on_push(int p, int l, int r) {
    acc[p] += lazy[p];
    if (l != -1) lazy[l] += lazy[p];
    if (r != -1) lazy[r] += lazy[p];
    lazy[p] = 0;
}

int main() {
    int N;
    std::cin >> N;
    acc.resize(N);
    lazy.resize(N);
    kotone::link_cut_tree<nullptr, nullptr, on_push> lct(N);
    for (int i = 1; i < N; i++) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        lct.reorient(u);
        lct.link(u, v);
    }
    int Q;
    std::cin >> Q;
    while (Q--) {
        int u, v;
        std::cin >> u >> v;
        u--, v--;
        lct.reorient(u);
        lct.access(v);
        lazy[v]++;
    }
    int64_t result = 0;
    for (int i = 0; i < N; i++) {
        lct.access(i);
        result += acc[i] * int64_t(acc[i] + 1) / 2;
    }
    std::cout << result << std::endl;
}
