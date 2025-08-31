#ifndef KOTONE_ITERTOOLS_HPP
#define KOTONE_ITERTOOLS_HPP 1

#include <vector>
#include <iterator>
#include <cassert>

namespace kotone {

namespace itertools {

template <typename T> struct product {
  private:
    const std::vector<T> &_vec;
    int _repeat;

  public:
    struct iterator {
        using value_type = std::vector<T>;
        using reference = const value_type&;
        using pointer = const value_type*;
        using iterator_category = std::input_iterator_tag;

      private:
        const std::vector<T> &_vec;
        std::vector<T> _current;
        std::vector<std::size_t> _indices;
        int _repeat;
        bool _is_end;

      public:
        iterator(const std::vector<T> &vec, int repeat, bool is_end)
            : _vec(vec),
              _current(repeat, vec.empty() ? T{} : vec[0]),
              _indices(repeat, 0),
              _repeat(repeat),
              _is_end(is_end)
        {}

        reference operator*() const {
            return _current;
        }

        pointer operator->() const {
            return &_current;
        }

        iterator& operator++() {
            if (_is_end) return *this;

            for (int i = _repeat - 1; i >= 0; i--) {
                if (++_indices[i] >= _vec.size()) {
                    _indices[i] = 0;
                    _current[i] = _vec[0];
                    continue;
                }

                _current[i] = _vec[_indices[i]];
                return *this;
            }

            _is_end = true;
            return *this;
        }

        iterator operator++(int) {
            iterator result = *this;
            ++*this;
            return result;
        }

        bool operator==(const iterator &other) const {
            return _is_end == other._is_end;
        }

        bool operator!=(const iterator &other) const {
            return !(*this == other);
        }
    };

    product(const std::vector<T> &vec, int repeat) : _vec(vec), _repeat(repeat) {
        assert(repeat >= 0);
    }

    iterator begin() const {
        return {_vec, _repeat, false};
    }

    iterator end() const {
        return {_vec, _repeat, true};
    }
};

}  // namespace itertools

}  // namespace kotone

#endif  // KOTONE_ITERTOOLS_HPP
