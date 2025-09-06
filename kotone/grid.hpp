#ifndef KOTONE_GRID_HPP
#define KOTONE_GRID_HPP 1

#include <utility>
#include <iterator>
#include <concepts>
#include <cassert>

namespace kotone {

// A helper class for grid-related problems.
template <std::integral T = int> struct grid {
  private:
    T _h, _w;
    static constexpr int _di[4] = {-1, 1, 0, 0};
    static constexpr int _dj[4] = {0, 0, -1, 1};
    static constexpr int _di_8[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    static constexpr int _dj_8[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

  public:
    grid() : _h(T{}), _w(T{}) {}

    // Initializes a grid with `num_rows` and `num_cols`.
    grid(T num_rows, T num_cols) : _h(num_rows), _w(num_cols) {
        assert(num_rows >= 0);
        assert(num_cols >= 0);
    }

    // Returns whether the given coordinates is within the grid,
    // i.e. whether `0 <= row < num_rows` and `0 <= col < num_cols`.
    bool is_within_grid(T row, T col) const {
        return T{} <= row && row < _h && T{} <= col && col < _w;
    }

    struct neighbors {
      private:
        const grid<T> &_g;
        T _i, _j;

      public:
        struct iterator {
            using value_type = std::pair<T, T>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

          private:
            const grid<T> &_g;
            T _i, _j;
            int _dir;

            void _skip() {
                while (_dir < 4) {
                    T ni = _i + _di[_dir];
                    T nj = _j + _dj[_dir];
                    if (_g.is_within_grid(ni, nj)) return;
                    _dir++;
                }
            }

          public:
            iterator(const grid<T> &grid, T row, T col, int dir)
                : _g(grid), _i(row), _j(col), _dir(dir)
            {
                _skip();
            }

            value_type operator*() const {
                return {_i + _di[_dir], _j + _dj[_dir]};
            }

            iterator& operator++() {
                _dir++;
                _skip();
                return *this;
            }

            iterator operator++(int) {
                iterator result = *this;
                ++*this;
                return result;
            }

            bool operator==(const iterator &other) const {
                return _dir == other._dir;
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }
        };

        neighbors(const grid<T> &grid, T row, T col) : _g(grid), _i(row), _j(col) {
            assert(grid.is_within_grid(row, col));
        }

        iterator begin() const {
            return {_g, _i, _j, 0};
        }

        iterator end() const {
            return {_g, _i, _j, 4};
        }
    };

    // Returns a container that iterates through the neighbors of the provided cell.
    // The order of iteration is undefined.
    // Only iterates through neighbors within the grid.
    neighbors neighbors(T row, T col) const {
        return {*this, row, col};
    }

    struct eight_neighbors {
      private:
        const grid<T> &_g;
        T _i, _j;

      public:
        struct iterator {
            using value_type = std::pair<T, T>;
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

          private:
            const grid<T> &_g;
            T _i, _j;
            int _dir;

            void _skip() {
                while (_dir < 8) {
                    T ni = _i + _di_8[_dir];
                    T nj = _j + _dj_8[_dir];
                    if (_g.is_within_grid(ni, nj)) return;
                    _dir++;
                }
            }

          public:
            iterator(const grid<T> &grid, T row, T col, int dir)
                : _g(grid), _i(row), _j(col), _dir(dir)
            {
                _skip();
            }

            value_type operator*() const {
                return {_i + _di_8[_dir], _j + _dj_8[_dir]};
            }

            iterator& operator++() {
                _dir++;
                _skip();
                return *this;
            }

            iterator operator++(int) {
                iterator result = *this;
                ++*this;
                return result;
            }

            bool operator==(const iterator &other) const {
                return _dir == other._dir;
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }
        };

        eight_neighbors(const grid<T> &grid, T row, T col) : _g(grid), _i(row), _j(col) {
            assert(grid.is_within_grid(row, col));
        }

        iterator begin() const {
            return {_g, _i, _j, 0};
        }

        iterator end() const {
            return {_g, _i, _j, 8};
        }
    };

    // Returns a container that iterates through the eight neighbors of the provided cell.
    // The order of iteration is undefined.
    // Only iterates through neighbors within the grid.
    eight_neighbors eight_neighbors(T row, T col) const {
        return {*this, row, col};
    }
};

}  // namespace kotone

#endif  // KOTONE_GRID_HPP
