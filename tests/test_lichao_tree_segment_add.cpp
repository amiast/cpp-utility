// Verified with: https://judge.yosupo.jp/problem/segment_add_get_min
// Details: https://judge.yosupo.jp/submission/323085

#include <iostream>
#include <vector>
#include <kotone/lichao_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int64_t> L(N), R(N), A(N), B(N);
    for (int i = 0; i < N; i++) std::cin >> L[i] >> R[i] >> A[i] >> B[i];
    std::vector<int64_t> T(Q), X(Q), Y(Q), Z(Q), W(Q), domain;
    for (int i = 0; i < Q; i++) {
        std::cin >> T[i] >> X[i];
        if (T[i] == 0) std::cin >> Y[i] >> Z[i] >> W[i];
        else domain.push_back(X[i]);
    }
    kotone::lichao_tree lct(domain);
    for (int i = 0; i < N; i++) lct.insert_segment(A[i], B[i], L[i], R[i]);
    for (int i = 0; i < Q; i++) {
        if (T[i] == 0) lct.insert_segment(Z[i], W[i], X[i], Y[i]);
        else {
            int64_t result = lct.find_min(X[i]);
            if (result == LLONG_MAX) std::cout << "INFINITY" << std::endl;
            else std::cout << result << std::endl;
        }
    }
}
