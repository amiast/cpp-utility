#ifndef KOTONE_MONOTONIC_QUEUE_HPP
#define KOTONE_MONOTONIC_QUEUE_HPP 1

#include <deque>
#include <cassert>

namespace kotone {

// A queue that maintains minimum/maximum values over a sliding window.
template <typename T, typename comp_pred = std::less<T>> struct monotonic_queue {
  private:
    std::deque<std::pair<T, int>> _deque;
    comp_pred _comp{};
    int _time_push = 0, _time_pop = 0;

  public:
    // Returns the number of elements in the queue.
    int size() const noexcept {
        return _time_push - _time_pop;
    }

    // Returns whether the queue is empty.
    bool empty() const noexcept {
        return _time_push == _time_pop;
    }

    // Pushes the specified value to the back of the queue.
    void push(T val) {
        while (_deque.size() && _comp(val, _deque.back().first)) _deque.pop_back();
        _deque.emplace_back(val, _time_push++);
    }

    // Pops the element at the front of the queue.
    // Requires the queue to be non-empty.
    void pop() {
        assert(!empty());
        _time_pop++;
        while (_deque.size() && _deque.front().second < _time_pop) _deque.pop_front();
    }

    // Returns the minimum/maximum value of the queue.
    // Requires the queue to be non-empty.
    T query() const {
        assert(!empty());
        return _deque.front().first;
    }

    // Erases all elements from the queue.
    void clear() {
        _time_push = _time_pop = 0;
        _deque.clear();
    }
};

}  // namespace kotone

#endif  // KOTONE_MONOTONIC_QUEUE_HPP
