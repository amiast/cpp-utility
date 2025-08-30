#ifndef KOTONE_MODINT_UTILITY_HPP
#define KOTONE_MODINT_UTILITY_HPP

#include <vector>
#include <cassert>

namespace kotone {

// Returns the vector of the first N factorials.
template <typename mint> std::vector<mint> factorials(int N) {
    assert(N >= 0);
    assert(N <= 100000000);
    std::vector<mint> result(N);
    if (N) result[0] = 1;
    for (int i = 1; i < N; i++) result[i] = i * result[i - 1];
    return result;
}

// Returns the vector of the first N inverse factorials.
template <typename mint> std::vector<mint> inv_factorials(const std::vector<mint> &factorials) {
    int N = static_cast<int>(factorials.size());
    std::vector<mint> result(N);
    if (N) {
        assert(factorials[N - 1] != 0);
        result[N - 1] = 1 / factorials[N - 1];
    }
    for (int i = N - 1; i > 0; i--) result[i - 1] = i * result[i];
    return result;
}

}  // namespace kotone

#endif  // KOTONE_MODINT_UTILITY_HPP
