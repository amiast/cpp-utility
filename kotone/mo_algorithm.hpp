#ifndef KOTONE_MO_ALGORITHM_HPP
#define KOTONE_MO_ALGORITHM_HPP 1

#include <vector>
#include <algorithm>

namespace kotone {

// Reference:
// https://take44444.github.io/Algorithm-Book/range/mo/main.html
struct mo_alg {
  private:
    int _q = 0;
    std::vector<int> _l, _r;

  public:
    void insert(int l, int r) {
        _q++;
        _l.emplace_back(l);
        _r.emplace_back(r);
    }

    template <typename order_, typename add_l_, typename add_r_, typename del_l_, typename del_r_, typename solve_>
    void execute(order_ &&order, add_l_ &&add_l, add_r_ && add_r, del_l_ &&del_l, del_r_ &&del_r, solve_ &&solve) {
        std::vector<int> indices(_q);
        std::vector<int64_t> ordering(_q);
        for (int i = 0; i < _q; i++) {
            indices[i] = i;
            ordering[i] = order(_l[i], _r[i]);
        }
        std::ranges::sort(indices, [&ordering](int i, int j){ return ordering[i] < ordering[j]; });
        int nl = 0, nr = 0;
        for (int i : indices) {
            while (nl > _l[i]) add_l(--nl, nr);
            while (nr < _r[i]) add_r(nl, nr++);
            while (nl < _l[i]) del_l(nl++, nr);
            while (nr > _r[i]) del_r(nl, --nr);
            solve(i);
        }
    }
};

// Reference:
// https://take44444.github.io/Algorithm-Book/range/mo/main.html
int64_t hilbert_index(int x, int y, int max_bit_width = 30) {
    int rx, ry, index = 0;
    int N = 1 << max_bit_width;
    for (int64_t s = N >> 1; s; s >>= 1) {
        rx = (x & s) > 0, ry = (y & s) > 0;
        index += s * s * ((rx * 3) ^ ry);
        if (ry) continue;
        if (rx) {
            x = N - 1 - x;
            y = N - 1 - y;
        }
        std::swap(x, y);
    }
    return index;
}

}  // namespace kotone

#endif  // KOTONE_MO_ALGORITHM_HPP
