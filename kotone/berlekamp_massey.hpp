#ifndef KOTONE_BERLEKAMP_MASSEY_HPP
#define KOTONE_BERLEKAMP_MASSEY_HPP 1

#include <vector>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// Returns a vector containing the minimum number of coefficients of the recurrence relation
// given the specified initial condition in `vec`.
template <compatible_modint mint> std::vector<mint> berlekamp_massey(const std::vector<mint> &vec) {
    assert(vec.size() <= 100000000u);
    int len = static_cast<int>(vec.size());
    std::vector<mint> coeffs{1}, last_coeffs{1};
    int curr_len = 0, num_steps = 1;
    mint last_diff = 1;
    for (int i = 0; i < len; i++) {
        mint diff = vec[i];
        for (int j = 1; j <= curr_len; j++) diff += coeffs[j] * vec[i - j];
        if (diff == 0) {
            num_steps++;
            continue;
        }
        std::vector<mint> temp = coeffs;
        mint factor = diff / last_diff;
        if (coeffs.size() < last_coeffs.size() + num_steps) {
            coeffs.resize(last_coeffs.size() + num_steps);
        }
        for (unsigned j{}; j < last_coeffs.size(); j++) {
            coeffs[j + num_steps] -= factor * last_coeffs[j];
        }
        if (curr_len * 2 <= i) {
            last_coeffs = temp;
            curr_len = i + 1 - curr_len;
            last_diff = diff;
            num_steps = 1;
        } else {
            num_steps++;
        }
    }
    coeffs.erase(coeffs.begin());
    for (mint &c : coeffs) c = -c;
    return coeffs;
}

}  // namespace kotone

#endif  // KOTONE_BERLEKAMP_MASSEY_HPP
