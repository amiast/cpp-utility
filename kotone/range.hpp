#ifndef KOTONE_RANGE_HPP
#define KOTONE_RANGE_HPP 1

#include <iterator>
#include <cassert>

namespace kotone {

template <typename T = int> struct range {
    static_assert(std::is_arithmetic<T>::value);

  private:
    T _start, _stop, _steps;

  public:
    struct iterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::input_iterator_tag;

      private:
        T _current, _stop, _steps;

      public:
        iterator(T current, T stop, T steps) : _current(current), _stop(stop), _steps(steps) {}

        reference operator*() const {
            return _current;
        }

        iterator& operator++() {
            _current = _current + _steps;
            return *this;
        }

        iterator operator++(int) {
            iterator result = *this;
            ++*this;
            return result;
        }

        bool operator!=(const iterator &other) const {
            if (_steps > T{}) return _current < other._current;
            return _current > other._current;
        }

        bool operator==(const iterator &other) const {
            return !(*this != other);
        }
    };

    range(T start, T stop, T steps = T{1}) : _start(start), _stop(stop), _steps(steps) {
        assert(steps != T{});
    }

    range(T stop = T{}) : range(T{}, stop) {}

    iterator end() const {
        return iterator(_stop, _stop, _steps);
    }

    iterator begin() const {
        if (_steps > T{} && _start >= _stop) return end();
        if (_steps < T{} && _start <= _stop) return end();
        return {_start, _stop, _steps};
    }
};

}  // namespace kotone

#endif  // KOTONE_RANGE_HPP
