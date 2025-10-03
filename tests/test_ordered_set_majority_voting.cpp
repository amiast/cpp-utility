// Verified with: https://judge.yosupo.jp/problem/majority_voting
// Details: https://judge.yosupo.jp/submission/318360

#include <iostream>
#include <vector>
#include <atcoder/segtree>
#include <kotone/coord_compress>
#include <kotone/ordered_set>

struct S {
    int candidate = -1, count = 0;
};

S op(S a, S b) {
    if (a.candidate == b.candidate) return {a.candidate, a.count + b.count};
    if (a.count >= b.count) return {a.candidate, a.count - b.count};
    return {b.candidate, b.count - a.count};
}

S e() {
    return {};
}

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<S> vec(N);
    for (auto &[candidate, count] : vec) std::cin >> candidate, count = 1;
    std::vector<int> T(Q), L(Q), R(Q);
    for (int i = 0; i < Q; i++) std::cin >> T[i] >> L[i] >> R[i];

    atcoder::segtree<S, op, e> seg(vec);
    kotone::coord_compress_hashmap<int> map;
    for (S &s : vec) map.insert(s.candidate);
    for (int i = 0; i < Q; i++) if (T[i] == 0) map.insert(R[i]);

    std::vector<kotone::ordered_set<int>> coords(map.size());
    for (int i = 0; i < N; i++) coords[map[vec[i].candidate]].insert(i);

    for (int i = 0; i < Q; i++) {
        if (T[i] == 0) {
            coords[map[seg.get(L[i]).candidate]].erase(L[i]);
            seg.set(L[i], {R[i], 1});
            coords[map[R[i]]].insert(L[i]);
        } else {
            int result = seg.prod(L[i], R[i]).candidate;
            int count = 0;
            if (result != -1) count = coords[map[result]].order_of(R[i]) - coords[map[result]].order_of(L[i]);
            if (count * 2 > R[i] - L[i]) {
                std::cout << result << std::endl;
            } else {
                std::cout << -1 << std::endl;
            }
        }
    }
}
