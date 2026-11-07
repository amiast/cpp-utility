#ifndef KOTONE_SUBSET_CONVOLUTION_HPP
#define KOTONE_SUBSET_CONVOLUTION_HPP 1

#include <vector>
#include <array>
#include <bit>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// A utility class for subset convolution.
// Reference: https://nyaannyaan.github.io/library/set-function/subset-convolution.hpp.html
template <mutable_additive T, unsigned MAX_LENGTH> struct subset_convolution {
    using fps = std::array<T, MAX_LENGTH + 1>;

  private:
    static void _add(fps &l, const fps &r, unsigned d) noexcept {
        for (unsigned i = 0; i < d; i++) l[i] += r[i];
    }

    static void _subtract(fps &l, const fps &r, unsigned d) noexcept {
        for (unsigned i = d; i <= MAX_LENGTH; i++) l[i] -= r[i];
    }

    static void _zeta(std::vector<fps> &SPS) noexcept {
        for (unsigned w = 1; w < SPS.size(); w *= 2) {
            for (unsigned k = 0; k < SPS.size(); k += w * 2) {
                for (unsigned i = 0; i < w; i++) {
                    _add(SPS[k + w + i], SPS[k + i], std::popcount(k + w + i));
                }
            }
        }
    }

    static void _moebius(std::vector<fps> &SPS) noexcept {
        for (unsigned w = SPS.size() / 2; w; w /= 2) {
            for (unsigned k = 0; k < SPS.size(); k += w * 2) {
                for (unsigned i = 0; i < w; i++) {
                    _subtract(SPS[k + w + i], SPS[k + i], std::popcount(k + w + i));
                }
            }
        }
    }

    static std::vector<fps> _to_fps(const std::vector<T> &sps) {
        std::vector<fps> SPS(sps.size());
        for (unsigned i = 0; i < sps.size(); i++) {
            SPS[i][std::popcount(i)] = sps[i];
        }
        return SPS;
    }

    static std::vector<T> _to_seq(const std::vector<fps> &SPS) {
        std::vector<T> sps(SPS.size());
        for (unsigned i = 0; i < SPS.size(); i++) {
            sps[i] = SPS[i][std::popcount(i)];
        }
        return sps;
    }

    static void _multiply(std::vector<fps> &l, const std::vector<fps> &r) noexcept {
        unsigned n = l.size(), d = std::countr_zero(n);
        for (unsigned i = 0; i < n; i++) {
            fps prod{};
            for (unsigned j = 0; j <= d; j++) {
                for (unsigned k = 0; k <= d - j; k++) {
                    prod[j + k] += l[i][j] * r[i][k];
                }
            }
            l[i] = prod;
        }
    }

  public:
    // Returns the convolution of the specified set power series.
    // Requires `0 < sps_l.size() == sps_r.size() <= 1 << MAX_LENGTH`.
    // Requires `sps_l.size()` and `sps_r.size()` to be powers of `2`.
    static std::vector<T> convolve(const std::vector<T> &sps_l, const std::vector<T> &sps_r) {
        assert(sps_l.size() == sps_r.size());
        assert(!sps_l.empty());
        assert(sps_l.size() <= 1u << MAX_LENGTH);
        assert(std::popcount(sps_l.size()) == 1);
        std::vector<fps> L = _to_fps(sps_l), R = _to_fps(sps_r);
        _zeta(L);
        _zeta(R);
        _multiply(L, R);
        _moebius(L);
        return _to_seq(L);
    }
};

}  // namespace kotone

#endif  // KOTONE_SUBSET_CONVOLUTION_HPP
