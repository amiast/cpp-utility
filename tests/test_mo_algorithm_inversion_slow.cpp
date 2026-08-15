// Verified with: https://judge.yosupo.jp/problem/static_range_inversions_query
// Details: https://judge.yosupo.jp/submission/393789

#include <iostream>
#include <vector>
#include <atcoder/fenwicktree>
#include <kotone/coord_compress>
#include <kotone/mo_algorithm>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int N, Q, M;
    std::cin >> N >> Q;
    std::vector<int> A(N);
    {
        kotone::coord_compress_compact<int> map;
        for (int &a : A) std::cin >> a, map.insert(a);
        map.build();
        M = map.size();
        for (int &a : A) a = map[a];
    }

    kotone::mo_alg mo;
    for (int i = 0; i < Q; i++) {
        int l, r;
        std::cin >> l >> r;
        mo.insert_query(l, r);
    }

    atcoder::fenwick_tree<int> bit(M);
    int64_t acc = 0;
    std::vector<int64_t> result(Q);
    auto incr_l = [&](int l, int) {
        acc -= bit.sum(0, A[l]);
        bit.add(A[l], -1);
    };
    auto decr_l = [&](int l, int) {
        acc += bit.sum(0, A[l]);
        bit.add(A[l], 1);
    };
    auto incr_r = [&](int, int r) {
        acc += bit.sum(A[r] + 1, M);
        bit.add(A[r], 1);
    };
    auto decr_r = [&](int, int r) {
        acc -= bit.sum(A[r] + 1, M);
        bit.add(A[r], -1);
    };
    auto solve = [&](int i) { result[i] = acc; };

    mo.eval_queries(kotone::zigzag_index<100001, 316>, incr_l, decr_l, incr_r, decr_r, solve);
    for (int64_t r : result) std::cout << r << '\n';
}
