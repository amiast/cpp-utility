// Verified with: https://judge.yosupo.jp/problem/mul_modp_convolution
// Details: https://judge.yosupo.jp/submission/366201

#include <iostream>
#include <atcoder/convolution>
#include <kotone/prime>

using mint = atcoder::modint998244353;

int main() {
    int P;
    std::cin >> P;
    std::vector<int> A(P), B(P);
    for (int &a : A) std::cin >> a;
    for (int &b : B) std::cin >> b;

    int64_t g = kotone::least_primitive_root(P);
    int64_t acc = 1;
    std::vector<int> exp_to_int(P);
    for (int i = 0; i < P - 1; i++) {
        exp_to_int[i] = acc;
        acc *= g;
        acc %= P;
    }

    std::vector<mint> pa(P - 1), pb(P - 1);
    for (int i = 0; i < P - 1; i++) pa[i] = A[exp_to_int[i]];
    for (int i = 0; i < P - 1; i++) pb[i] = B[exp_to_int[i]];
    std::vector<mint> prod = atcoder::convolution(pa, pb);

    std::vector<mint> result(P);
    for (int i = 0; i < P; i++) result[0] += mint(A[0]) * B[i];
    for (int i = 1; i < P; i++) result[0] += mint(B[0]) * A[i];
    for (int i = 0; i < P * 2 - 3; i++) result[exp_to_int[i % (P - 1)]] += prod[i];
    for (mint r : result) std::cout << r.val() << std::endl;
}
