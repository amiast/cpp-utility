#ifndef RETROACTIVE_SEGTREE_HPP
#define RETROACTIVE_SEGTREE_HPP 1

#include <iostream>
#include <vector>
#include <queue>
#include <utility>

template <typename T>
struct erasable_pq {
    std::priority_queue<T> _pq, _lazy;

    void push(T x) {
        _pq.emplace(x);
    }

    void erase(T x) {
        _lazy.emplace(x);
    }

    T top() {
        while (_lazy.size() && _pq.top() == _lazy.top()) {
            _pq.pop();
            _lazy.pop();
        }
        if (_pq.size()) return _pq.top();
        return T{};
    }
};

template <typename T>
struct retroactive_segtree {
    int _N;
    std::vector<erasable_pq<T>> _intervals;

    retroactive_segtree(std::vector<T> &v) {
        _N = v.size();
        _intervals.resize(_N * 2);
        for (int i = 0; i < _N; i++) {
            _intervals[i + _N].push(v[i]);
        }
    }

    void push_update(int l, int r, T x) {
        l += _N;
        r += _N;
        while (l != r) {
            if (l & 1) _intervals[l++].push(x);
            if (r & 1) _intervals[--r].push(x);
            l >>= 1;
            r >>= 1;
        }
    }

    void erase_update(int l, int r, T x) {
        l += _N;
        r += _N;
        while (l != r) {
            if (l & 1) _intervals[l++].erase(x);
            if (r & 1) _intervals[--r].erase(x);
            l >>= 1;
            r >>= 1;
        }
    }

    T get(int i, const auto &aggregate) {
        i += _N;
        T result{};
        while (i) {
            result = aggregate(result, _intervals[i].top());
            i >>= 1;
        }
        return result;
    }
};

// Example - https://atcoder.jp/contests/abc342/tasks/abc342_g

int main() {
    int N;
    std::cin >> N;
    std::vector<int> vec(N);
    for (int &v : vec) std::cin >> v;
    retroactive_segtree seg(vec);

    int Q;
    std::cin >> Q;
    std::vector<int> query_l(Q), query_r(Q), query_x(Q);
    for (int i = 0; i < Q; i++) {
        int t;
        std::cin >> t;
        if (t == 1) {
            std::cin >> query_l[i] >> query_r[i] >> query_x[i];
            query_l[i]--;
            seg.push_update(query_l[i], query_r[i], query_x[i]);
        } else if (t == 2) {
            int i;
            std::cin >> i;
            i--;
            seg.erase_update(query_l[i], query_r[i], query_x[i]);
        } else {
            int i;
            std::cin >> i;
            i--;
            std::cout << seg.get(i, [](int a, int b){ return a > b ? a : b; }) << std::endl;
        }
    }
}

#endif  // RETROACTIVE_SEGTREE
