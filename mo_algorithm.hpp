#ifndef MO_ALGORITHM_HPP
#define MO_ALGORITHM_HPP 1

#include <vector>
#include <algorithm>

struct mo_alg {
    int _q = 0;
    std::vector<int> _l, _r;

    void insert(int l, int r) {
        _q++;
        _l.emplace_back(l);
        _r.emplace_back(r);
    }

    template <typename sort_, typename add_l_, typename add_r_, typename del_l_, typename del_r_, typename solve_>
    void execute(sort_ &&sort, add_l_ &&add_l, add_r_ && add_r, del_l_ &&del_l, del_r_ &&del_r, solve_ &&solve) {
        std::vector<int> indices(_q);
        std::vector<int64_t> order(_q);
        for (int i = 0; i < _q; i++) {
            indices[i] = i;
            order[i] = sort(_l[i], _r[i]);
        }
        std::ranges::sort(indices, [&order](int i, int j){ return order[i] < order[j]; });
        int nl = 0, nr = 0;
        for (int i : indices) {
            while (nl > _l[i]) add_l(--nl);
            while (nr < _r[i]) add_r(nr++);
            while (nl < _l[i]) del_l(nl++);
            while (nr > _r[i]) del_r(--nr);
            solve(i);
        }
    }
};

#endif  // MO_ALGORITHM_HPP
