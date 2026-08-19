// Verified with: https://judge.yosupo.jp/problem/static_range_mode_query
// Details: https://judge.yosupo.jp/submission/394847

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <kotone/mo_algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;
    kotone::mo_alg mo;
    for (int i = 0; i < Q; i++) {
        int l, r;
        std::cin >> l >> r;
        mo.insert_query(l, r);
    }

    std::vector<std::unordered_set<int>> occ(N + 1);
    occ[0] = {A.begin(), A.end()};
    std::list<int> list{0};
    std::vector<std::list<int>::iterator> pos(N + 1);
    pos[0] = list.begin();
    std::unordered_map<int, int> freq;

    std::vector<std::pair<int, int>> result(Q);
    auto solve = [&](int i) {
        result[i].second = list.back();
        result[i].first = *occ[result[i].second].begin();
    };
    auto add = [&](int i) {
        int c = freq[A[i]]++;
        if (occ[c + 1].empty()) pos[c + 1] = list.insert(std::next(pos[c]), c + 1);
        occ[c + 1].insert(A[i]);
        occ[c].erase(A[i]);
        if (occ[c].empty()) list.erase(pos[c]);
    };
    auto del = [&](int i) {
        int c = freq[A[i]]--;
        if (occ[c - 1].empty()) pos[c - 1] = list.insert(pos[c], c - 1);
        occ[c - 1].insert(A[i]);
        occ[c].erase(A[i]);
        if (occ[c].empty()) list.erase(pos[c]);
    };

    mo.eval_queries_add_del(kotone::zigzag_index<100001, 317>, add, del, solve);
    for (auto [v, c] : result) std::cout << v << ' ' << c << '\n';
}
