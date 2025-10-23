// Verified with: https://judge.yosupo.jp/problem/line_add_get_min
// Details: https://judge.yosupo.jp/submission/323084

#include <iostream>
#include <vector>
#include <kotone/lichao_tree>

int main() {
    int N, Q;
    std::cin >> N >> Q;
    std::vector<int64_t> A(N), B(N);
    for (int i = 0; i < N; i++) std::cin >> A[i] >> B[i];
    std::vector<int64_t> T(Q), X(Q), Y(Q), domain;
    for (int i = 0; i < Q; i++) {
        std::cin >> T[i] >> X[i];
        if (T[i] == 0) std::cin >> Y[i];
        else domain.push_back(X[i]);
    }
    kotone::lichao_tree lct(domain);
    for (int i = 0; i < N; i++) lct.insert(A[i], B[i]);
    for (int i = 0; i < Q; i++) {
        if (T[i] == 0) lct.insert(X[i], Y[i]);
        else std::cout << lct.find_min(X[i]) << std::endl;
    }
}
