// Verified with: https://judge.yosupo.jp/problem/multipoint_evaluation
// Details: https://judge.yosupo.jp/submission/387957

#include <iostream>
#include <kotone/formal_power_series>

using mint = atcoder::modint998244353;
using fps = kotone::formal_power_series<mint>;

int main() {
    int N, M;
    std::cin >> N >> M;
    fps C(N);
    std::vector<mint> P(M);
    for (int i = 0; i < N; i++) {
        int c;
        std::cin >> c;
        C[i] = c;
    }
    for (int i = 0; i < M; i++) {
        int p;
        std::cin >> p;
        P[i] = p;
    }
    for (mint &m : C.eval(P)) std::cout << m.val() << ' ';
    std::cout << std::endl;
}
