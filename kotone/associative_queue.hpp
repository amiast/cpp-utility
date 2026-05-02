#ifndef KOTONE_ASSOCIATIVE_QUEUE_HPP
#define KOTONE_ASSOCIATIVE_QUEUE_HPP 1

#include <deque>
#include <cassert>

namespace kotone {

// A queue that maintains associative product over a sliding window.
template <typename S, S (*op)(S, S)> struct associative_queue {
  private:
    std::deque<S> _deque;
    S _sfx;
    int _mid = 0;

  public:
    // Returns the number of elements in the queue.
    int size() const noexcept {
        return _deque.size();
    }

    // Returns whether the queue is empty.
    bool empty() const noexcept {
        return _deque.empty();
    }

    // Pushes the specified value to the back of the queue.
    void push(S val) {
        _sfx = _mid == size() ? val : op(_sfx, val);
        _deque.push_back(val);
    }

    // Pops the element at the front of the queue.
    // Requires the queue to be non-empty.
    void pop() {
        assert(!empty());
        if (_mid == 0) {
            _mid = _deque.size();
            for (int i = _mid - 1; i > 0; i--) {
                _deque[i - 1] = op(_deque[i - 1], _deque[i]);
            }
        }
        _deque.pop_front();
        _mid--;
    }

    // Returns the product of elements in the queue.
    // Requires the queue to be non-empty.
    S query() const {
        assert(!empty());
        if (_mid == 0) return _sfx;
        if (_mid == size()) return _deque.front();
        return op(_deque.front(), _sfx);
    }

    // Erases all elements from the queue.
    void clear() {
        _deque.clear();
        _mid = 0;
    }
};

}  // namespace kotone

#endif  // KOTONE_ASSOCIATIVE_QUEUE_HPP
