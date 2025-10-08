#ifndef KOTONE_CONVOLUTION_REAL_HPP
#define KOTONE_CONVOLUTION_REAL_HPP 1

#include <vector>
#include <complex>
#include <cmath>
#include <numbers>
#include <bit>
#include <concepts>
#include <cassert>

namespace kotone {

// Performs a fast Fourier transform (or its inverse) on a vector of complex numbers.
// Requires the size of `fps` to be a power of `2`.
template <std::floating_point T> void fast_fourier_transform(std::vector<std::complex<T>> &fps, bool invert) {
    int n = static_cast<int>(fps.size());
    if (n <= 1) return;
    std::vector<std::complex<T>> fps0(n / 2), fps1(n / 2);
    for (int i = 0; i * 2 < n; i++) {
        fps0[i] = fps[i * 2];
        fps1[i] = fps[i * 2 + 1];
    }
    fast_fourier_transform(fps0, invert);
    fast_fourier_transform(fps1, invert);

    T arg = std::numbers::pi_v<T> * 2 / n;
    if (invert) arg = -arg;
    std::complex<T> root(1), root_primitive(std::cos(arg), std::sin(arg));
    for (int i = 0; i * 2 < n; i++) {
        std::complex<T> u = fps0[i], v = root * fps1[i];
        fps[i] = u + v;
        fps[i + n / 2] = u - v;
        root *= root_primitive;
    }

    if (invert) {
        for (std::complex<T> &c : fps) c /= 2;
    }
}

// Computes the convolution of two real-valued formal power series via fast Fourier transform.
// If either vector is empty, returns an empty vector.
template <std::floating_point T> std::vector<T> convolution(const std::vector<T> &fps_l, const std::vector<T> &fps_r) {
    if (fps_l.empty() || fps_r.empty()) return {};
    int len = 1 << std::bit_width(fps_l.size() + fps_r.size() - 1);
    int len_l = static_cast<int>(fps_l.size()), len_r = static_cast<int>(fps_r.size());
    std::vector<std::complex<T>> nl(len), nr(len);
    for (int i = 0; i < len_l; i++) nl[i] = fps_l[i];
    for (int i = 0; i < len_r; i++) nr[i] = fps_r[i];
    nl.resize(len);
    nr.resize(len);
    fast_fourier_transform(nl, false);
    fast_fourier_transform(nr, false);
    for (int i = 0; i < len; i++) nl[i] *= nr[i];
    fast_fourier_transform(nl, true);
    std::vector<T> result(len_l + len_r - 1);
    for (int i = 0; i < len_l + len_r - 1; i++) result[i] = nl[i].real();
    return result;
}

// Returns the inverse of the formal power series up to the first `n` coefficients.
// Requires `!fps.empty() && fps[0] != 0`.
// Requires `0 <= n <= 100000000`.
template <std::floating_point T> std::vector<T> inv_fps(const std::vector<T> &fps, int n) {
    assert(!fps.empty() && fps[0]);
    assert(0 <= n && n <= 100000000);
    if (n == 0) return {};
    std::vector<T> result{1 / fps[0]};
    int m = 1;
    int len = static_cast<int>(fps.size());
    while (m < n) {
        m = std::min(m * 2, n);
        std::vector<T> sub(fps.begin(), fps.begin() + std::min(m, len));
        if (m > len) sub.resize(m);
        std::vector<T> prod = convolution(result, sub);
        prod.resize(m);
        prod[0] = 2 - prod[0];
        for (int i = 1; i < m; i++) prod[i] = -prod[i];
        result = convolution(result, prod);
        result.resize(m);
    }
    result.resize(n);
    return result;
}

}  // namespace kotone

#endif  // KOTONE_CONVOLUTION_REAL_HPP
