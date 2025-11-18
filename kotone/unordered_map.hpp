#ifndef KOTONE_UNORDERED_MAP_HPP
#define KOTONE_UNORDERED_MAP_HPP 1

#include <vector>
#include <utility>
#include <iterator>
#include <concepts>
#include <random>
#include <chrono>

namespace kotone {

// A randomized hash for integers.
struct randomized_hash {
  private:
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

  public:
    std::size_t operator()(uint64_t x) const {
        static const uint64_t SEED = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x ^ SEED);
    }
};

// An collision-resistant unordered map.
template <std::integral S, typename T> struct unordered_map {
  private:
    enum bucket_state { EMPTY, FILLED, ERASED };
    struct bucket {
        std::pair<S, T> data;
        bucket_state state = EMPTY;
    };

    int _size = 0;
    std::vector<bucket> _table;
    std::size_t _load_size{};
    randomized_hash _hash;

    static std::size_t _next_pow_2(std::size_t n) {
        std::size_t p = 1ULL;
        while (p < n) p <<= 1;
        return p;
    }

    void _reallocate(std::size_t new_capacity) {
        new_capacity = _next_pow_2(new_capacity);
        std::vector<bucket> temp = std::move(_table);
        _table.assign(new_capacity, {});
        _load_size = 0ULL;
        for (bucket &b : temp) {
            if (b.state != FILLED) continue;
            std::size_t i = _hash(b.data.first) & (new_capacity - 1);
            while (_table[i].state == FILLED) i = (i + 1) & (new_capacity - 1);
            _table[i].data = std::move(b.data);
            _table[i].state = FILLED;
            _load_size++;
        }
    }

  public:
    // Constructs an empty hash map.
    unordered_map() : _table(8) {}

    // Returns a reference to the value associated with the specified key.
    T& operator[](S key) {
        if ((_load_size + 1) * 10 > _table.size() * 7) _reallocate(_table.size() * 2);
        std::size_t n = _table.size();
        std::size_t i = _hash(key) & (n - 1);
        std::size_t first_erased = n;
        while (true) {
            if (_table[i].state == EMPTY) {
                std::size_t index = first_erased == n ? i : first_erased;
                _table[index].data = {key, {}};
                _table[index].state = FILLED;
                _size++;
                _load_size++;
                return _table[index].data.second;
            }
            if (_table[i].state == ERASED && first_erased == n) first_erased = i;
            if (_table[i].state == FILLED && _table[i].data.first == key) return _table[i].data.second;
            i = (i + 1) & (n - 1);
        }
    }

    // Removes the specified key and returns whether the key has been newly erased.
    bool erase(S key) {
        std::size_t n = _table.size();
        std::size_t i = _hash(key) & (n - 1);
        while (true) {
            if (_table[i].state == EMPTY) return false;
            if (_table[i].state == FILLED && _table[i].data.first == key) {
                _table[i].state = ERASED;
                _size--;
                return true;
            }
            i = (i + 1) & (n - 1);
        }
    }

    // Returns whether the map contains the specified key.
    bool contains(S key) const {
        std::size_t n = _table.size();
        std::size_t i = _hash(key) & (n - 1);
        while (true) {
            if (_table[i].state == EMPTY) return false;
            if (_table[i].state == FILLED && _table[i].data.first == key) return true;
            i = (i + 1) & (n - 1);
        }
    }

    // Returns the number of key-value pairs in the map.
    int size() const {
        return _size;
    }

    // Returns whether the map is empty.
    bool empty() const {
        return _size == 0;
    }

    struct iterator {
        using value_type = std::pair<const S, T>;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

      private:
        bucket *_ptr, *_end;

        void _advance() { while (_ptr != _end && _ptr->state != FILLED) ++_ptr; }

      public:
        iterator(bucket *p, bucket *e) : _ptr(p), _end(e) { _advance(); }

        reference operator*() const { return _ptr->data; }
        pointer operator->() const { return &_ptr->data; }

        iterator& operator++() {
            ++_ptr;
            _advance();
            return *this;
        }

        iterator operator++(int) {
            iterator result = *this;
            ++*this;
            return result;
        }

        bool operator==(const iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const iterator &other) const { return !(*this == other); }
    };

    // Returns an iterator to the first element in the map.
    iterator begin() {
        return iterator(_table.data(), _table.data() + _table.size());
    }

    // Returns an iterator to the past-the-end element in the map.
    iterator end() {
        return iterator(_table.data() + _table.size(), _table.data() + _table.size());
    }

    // Returns an iterator to the specified key-value pair if it exists,
    // otherwise returns an iterator to `unordered_map::end`.
    iterator find(S key) {
        std::size_t n = _table.size();
        std::size_t i = _hash(key) & (n - 1);
        while (true) {
            if (_table[i].state == EMPTY) return end();
            if (_table[i].state == FILLED && _table[i].data.first == key) {
                return iterator(&_table[i], _table.data() + _table.size());
            }
            i = (i + 1) & (n - 1);
        }
    }

    struct const_iterator {
        using value_type = std::pair<const S, T>;
        using reference = const value_type&;
        using pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

      private:
        const bucket *_ptr, *_end;

        void _advance() { while (_ptr != _end && _ptr->state != FILLED) ++_ptr; }

      public:
        const_iterator(const bucket *p, const bucket *e) : _ptr(p), _end(e) { _advance(); }

        reference operator*() const { return reinterpret_cast<const value_type&>(_ptr->data); }
        pointer operator->() const { return reinterpret_cast<const value_type*>(&_ptr->data); }

        const_iterator& operator++() {
            ++_ptr;
            _advance();
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator result = *this;
            ++*this;
            return result;
        }

        bool operator==(const const_iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const const_iterator &other) const { return !(*this == other); }
    };

    // Returns a const_iterator to the first element in the map.
    const_iterator begin() const {
        return const_iterator(_table.data(), _table.data() + _table.size());
    }

    // Returns a const_iterator to the past-the-end element in the map.
    const_iterator end() const {
        return const_iterator(_table.data() + _table.size(), _table.data() + _table.size());
    }

    // Returns a const_iterator to the specified key-value pair if it exists,
    // otherwise returns a const_iterator to `unordered_map::end`.
    const_iterator find(S key) const {
        std::size_t n = _table.size();
        std::size_t i = _hash(key) & (n - 1);
        while (true) {
            if (_table[i].state == EMPTY) return end();
            if (_table[i].state == FILLED && _table[i].data.first == key) {
                return const_iterator(&_table[i], _table.data() + _table.size());
            }
            i = (i + 1) & (n - 1);
        }
    }
};

}  // namespace kotone

#endif  // KOTONE_UNORDERED_MAP_HPP
