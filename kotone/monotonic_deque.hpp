#ifndef KOTONE_MONOTONIC_DEQUE_HPP
#define KOTONE_MONOTONIC_DEQUE_HPP 1

#include <deque>
#include <cassert>

namespace kotone {

// A deque that maintains minimum/maximum values over a sliding window.
template <typename T, typename comp_pred = std::less<T>> struct monotonic_deque {
  private:
    int _time_push = 0, _time_pop = 0;
    std::deque<std::pair<T, int>> _deque;
    comp_pred _comp{};

  public:
    // Returns the number of elements in the deque.
    int size() const noexcept {
        return _time_push - _time_pop;
    }

    // Returns whether the deque is empty.
    bool empty() const noexcept {
        return _time_push == _time_pop;
    }

    // Pushes the specified value to the back of the deque.
    void push(T val) {
        while (_deque.size() && _comp(val, _deque.back().first)) _deque.pop_back();
        _deque.emplace_back(val, _time_push++);
    }

    // Pops the element at the front of the deque.
    // Requires the deque to be non-empty.
    void pop() {
        assert(!empty());
        _time_pop++;
        while (_deque.size() && _deque.front().second < _time_pop) _deque.pop_front();
    }

    // Returns the minimum/maximum value of the deque.
    // Requires the deque to be non-empty.
    T query() const {
        assert(!empty());
        return _deque.front().first;
    }
};

}  // namespace kotone

#endif  // KOTONE_MONOTONIC_DEQUE_HPP
