#ifndef MO_ALGORITHM_HPP
#define MO_ALGORITHM_HPP 1

#include <vector>
#include <algorithm>

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

#endif  // MO_ALGORITHM_HPP
