#ifndef KOTONE_GCD_CONVOLUTION_HPP
#define KOTONE_GCD_CONVOLUTION_HPP 1

#include <vector>

namespace kotone {

// Performs the specified multiple/divisor zeta/Moebius transform in place.
template <typename T, bool multiple, bool zeta> void divisor_transform(std::vector<T> &A) {
    int M = A.size();
    std::vector<bool> is_prime(M, true);
    auto op = [&](int i, int j){
        if constexpr (multiple) is_prime[j] = false;
        else is_prime[i] = false;
        if constexpr (zeta) A[i] += A[j];
        else A[i] -= A[j];
    };
    for (int p = 2; p < M; p++) {
        if (!is_prime[p]) continue;
        int q = (M - 1) / p;
        if constexpr (multiple) {
            if constexpr (zeta) for (int k = q; k > 0; k--) op(k, p * k);
            else for (int k = 1; k <= q; k++) op(k, p * k);
        } else {
            if constexpr (zeta) for (int k = 1; k <= q; k++) op(p * k, k);
            else for (int k = q; k > 0; k--) op(p * k, k);
        }
    }
}

// Returns the specified GCD/LCM convolution.
template <typename T, bool gcd> std::vector<T> divisor_convolution(std::vector<T> &&A, std::vector<T> &&B) {
    int M = A.size();
    divisor_transform<T, gcd, true>(A);
    divisor_transform<T, gcd, true>(B);
    std::vector<T> C(M);
    for (int i = 1; i < M; i++) C[i] = A[i] * B[i];
    divisor_transform<T, gcd, false>(C);
    return C;
}

}  // namespace kotone

#endif  // KOTONE_GCD_CONVOLUTION_HPP
