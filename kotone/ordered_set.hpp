#ifndef KOTONE_ORDERED_SET_HPP
#define KOTONE_ORDERED_SET_HPP 1

#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <kotone/memory_pool>

namespace kotone {

// An ordered set implemented with an AVL tree.
template <typename T, typename comp_pred = std::less<T>> struct ordered_set {
  private:
    struct node {
        T _val;
        int _height = 1, _size = 1;
        node *_left = nullptr, *_right = nullptr, *_parent = nullptr;
        node(const T& val) : _val(val) {}
        node(T &&val) : _val(std::move(val)) {}
    };

    struct eq_pred {
        comp_pred comp{};
        bool operator()(const T &a, const T &b) const {
            return !comp(a, b) && !comp(b, a);
        }
    };

    static constexpr int _DEFAULT_CHUNK_SIZE = 8;
    memory_pool<node> _pool{_DEFAULT_CHUNK_SIZE};
    node *_root = nullptr, *_min_node = nullptr, *_max_node = nullptr;
    comp_pred _comp{};
    eq_pred _eq{};

    int _height(node *root) const noexcept {
        return root ? root->_height : 0;
    }

    int _size(node *root) const noexcept {
        return root ? root->_size : 0;
    }

    void _update(node *root) noexcept {
        if (!root) return;
        root->_height = std::max(_height(root->_left), _height(root->_right)) + 1;
        root->_size = _size(root->_left) + _size(root->_right) + 1;
    }

    node* _rotate_left(node *root) noexcept {
        node *new_root = root->_right, *temp = new_root->_left;
        new_root->_left = root;
        root->_right = temp;
        if (temp) temp->_parent = root;
        new_root->_parent = root->_parent;
        root->_parent = new_root;
        _update(root);
        _update(new_root);
        return new_root;
    }

    node* _rotate_right(node *root) noexcept {
        node *new_root = root->_left, *temp = new_root->_right;
        new_root->_right = root;
        root->_left = temp;
        if (temp) temp->_parent = root;
        new_root->_parent = root->_parent;
        root->_parent = new_root;
        _update(root);
        _update(new_root);
        return new_root;
    }

    int _balance_factor(node *root) const noexcept {
        return root ? _height(root->_left) - _height(root->_right) : 0;
    }

    node* _balance(node *root) noexcept {
        if (!root) return root;
        _update(root);
        int factor = _balance_factor(root);
        if (factor == 0) return root;
        if (factor > 0) {
            if (_balance_factor(root->_left) < 0) root->_left = _rotate_left(root->_left);
            return _rotate_right(root);
        }
        if (_balance_factor(root->_right) > 0) root->_right = _rotate_right(root->_right);
        return _rotate_left(root);
    }

    template <typename U>
    std::pair<node*, bool> _insert(node *root, U &&val, node *parent = nullptr) {
        if (!root) {
            node *new_node = _pool.allocate(std::forward<U>(val));
            new_node->_parent = parent;
            if (!_min_node || _comp(val, _min_node->_val)) _min_node = new_node;
            if (!_max_node || _comp(_max_node->_val, val)) _max_node = new_node;
            return {new_node, true};
        }
        if (_eq(val, root->_val)) return {root, false};
        std::pair<node*, bool> result;
        if (_comp(val, root->_val)) {
            auto [child, inserted] = _insert(root->_left, val, root);
            if (inserted) {
                root->_left = child;
                root = _balance(root);
            }
            return {root, inserted};
        }
        auto [child, inserted] = _insert(root->_right, val, root);
        if (inserted) {
            root->_right = child;
            root = _balance(root);
        }
        return {root, inserted};
    }

    node* _get_min(node *root) const noexcept {
        while (root && root->_left) root = root->_left;
        return root;
    }

    node* _get_max(node *root) const noexcept {
        while (root && root->_right) root = root->_right;
        return root;
    }

    std::pair<node*, bool> _erase(node* root, const T &val) {
        if (!root) return {root, false};
        if (_eq(val, root->_val)) {
            if (_eq(val, _min_node->_val)) {
                if (root->_right) _min_node = _get_min(root->_right);
                else _min_node = root->_parent;
            }
            if (_eq(val, _max_node->_val)) {
                if (root->_left) _max_node = _get_max(root->_left);
                else _max_node = root->_parent;
            }
            if (!root->_left) {
                node *new_root = root->_right;
                if (new_root) new_root->_parent = root->_parent;
                _pool.deallocate(root);
                return {new_root, true};
            }
            if (!root->_right) {
                node *new_root = root->_left;
                new_root->_parent = root->_parent;
                _pool.deallocate(root);
                return {new_root, true};
            }
            node *min_node = _get_min(root->_right);
            root->_val = min_node->_val;
            root->_right = _erase(root->_right, min_node->_val).first;
            root = _balance(root);
            return {root, true};
        }
        if (_comp(val, root->_val)) {
            auto [new_left, erased] = _erase(root->_left, val);
            if (erased) {
                root->_left = new_left;
                root = _balance(root);
            }
            return {root, erased};
        }
        auto [new_right, erased] = _erase(root->_right, val);
        if (erased) {
            root->_right = new_right;
            root = _balance(root);
        }
        return {root, erased};
    }

    node* _find(node *root, const T &val) const {
        if (!root) return root;
        if (_eq(val, root->_val)) return root;
        if (_comp(val, root->_val)) return _find(root->_left, val);
        return _find(root->_right, val);
    }

    node* _get_nth(node *root, int index) const noexcept {
        if (!root) return root;
        int l_size = _size(root->_left);
        if (index == l_size) return root;
        if (index < l_size) return _get_nth(root->_left, index);
        return _get_nth(root->_right, index - l_size - 1);
    }

    int _order_of(node *root, const T &val) const {
        if (!root) return 0;
        if (_eq(val, root->_val)) return _size(root->_left);
        if (_comp(val, root->_val)) return _order_of(root->_left, val);
        return _size(root->_left) + _order_of(root->_right, val) + 1;
    }

    node* _build_sorted(const std::vector<T> &vec, int l, int r, node *parent = nullptr) noexcept {
        if (l >= r) return nullptr;
        int m = (l + r) / 2;
        node *root = _pool.allocate(vec[m]);
        root->_left = _build_sorted(vec, l, m, root);
        root->_right = _build_sorted(vec, m + 1, r, root);
        root->_parent = parent;
        _update(root);
        return root;
    }

    void _clear(node *root) {
        if (!root) return;
        _clear(root->_left);
        _clear(root->_right);
        _pool.deallocate(root);
    }

  public:
    // Constructs an empty set.
    ordered_set() noexcept {}

    // Constructs a set from elements of a brace-enclosed initializer list.
    ordered_set(std::initializer_list<T> init_list) {
        for (const T &val : init_list) insert(val);
    }

    // Constructs a set from a sorted vector of distinct elements.
    ordered_set(const std::vector<T> &sorted_vec) {
        int len = static_cast<int>(sorted_vec.size());
        assert(len <= 100000000);
        if (len) {
            for (typename std::vector<T>::const_iterator iter = sorted_vec.begin(); std::next(iter) != sorted_vec.end(); iter++) {
                assert(_comp(*iter, *std::next(iter)));
            }
        }
        if (len > _DEFAULT_CHUNK_SIZE) update_chunk_size(len);
        _root = _build_sorted(sorted_vec, 0, len);
        _min_node = _get_min(_root);
        _max_node = _get_max(_root);
        update_chunk_size(_DEFAULT_CHUNK_SIZE);
    }

    struct iterator {
        using value_type = T;
        using reference = const value_type&;
        using pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

      private:
        const ordered_set &_set;
        node *_curr;

      public:
        iterator(const ordered_set &set, node *node) noexcept : _set(set), _curr(node) {}
        iterator(const ordered_set &set) noexcept : _set(set), _curr(nullptr) {}

        reference operator*() const {
            assert(_curr);
            return _curr->_val;
        }

        pointer operator->() const {
            assert(_curr);
            return &_curr->_val;
        }

        bool operator==(const iterator &other) const noexcept {
            return &_set == &other._set && _curr == other._curr;
        }

        bool operator!=(const iterator &other) const noexcept {
            return !(*this == other);
        }

        iterator& operator++() {
            assert(_curr);
            if (_curr->_right) {
                _curr = _set._get_min(_curr->_right);
            } else {
                node *par = _curr->_parent;
                while (par && _curr == par->_right) {
                    _curr = par;
                    par = par->_parent;
                }
                _curr = par;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator result = *this;
            ++*this;
            return result;
        }

        iterator& operator--() {
            assert(_curr != _set._min_node);
            assert(_curr || !_set.empty());
            if (!_curr) {
                _curr = _set._get_max(_set._root);
            } else if (_curr->_left) {
                _curr = _set._get_max(_curr->_left);
            } else {
                node *par = _curr->_parent;
                while (par && _curr == par->_left) {
                    _curr = par;
                    par = par->_parent;
                }
                _curr = par;
            }
            return *this;
        }

        iterator operator--(int) {
            iterator result = *this;
            --*this;
            return result;
        }
    };

    // Returns an iterator to the first element in the set.
    iterator begin() const noexcept {
        return iterator(*this, _min_node);
    }

    // Returns an iterator to the past-the-end element in the set.
    iterator end() const noexcept {
        return iterator(*this);
    }

    // Returns the number of elements in the set.
    int size() const noexcept {
        return _size(_root);
    }

    // Returns whether the set is empty.
    bool empty() const noexcept {
        return _root == nullptr;
    }

    // Suggests a new chunk size for the memory pool used by the set.
    // Requires `chunk_size` to be a positive integer.
    void update_chunk_size(int chunk_size) {
        _pool.update_chunk_size(chunk_size);
    }

    // Inserts the specified value into the set, then returns a pair of:
    // - an iterator to the value in the set
    // - whether the value has been newly inserted
    std::pair<iterator, bool> insert(const T &val) {
        auto [new_root, inserted] = _insert(_root, val);
        _root = new_root;
        return {iterator(*this, _find(_root, val)), inserted};
    }

    // Inserts the specified rvalue into the set, then returns a pair of:
    // - an iterator to the value in the set
    // - whether the value has been newly inserted
    std::pair<iterator, bool> insert(T &&val) {
        auto [new_root, inserted] = _insert(_root, std::move(val));
        _root = new_root;
        return {iterator(*this, _find(_root, val)), inserted};
    }

    // Inserts the specified value in place using args for construction, then returns a pair of:
    // - an iterator to the value in the set
    // - whether the value has been newly inserted
    template <typename ...Args>
    std::pair<iterator, bool> emplace(Args &&...args) {
        T val(std::forward<Args>(args)...);
        auto [new_root, inserted] = _insert(_root, std::move(val));
        _root = new_root;
        return {iterator(*this, _find(_root, val)), inserted};
    }

    // Removes the specified value from the set,
    // then returns whether the value has been newly erased.
    bool erase(const T &val) {
        auto [new_root, erased] = _erase(_root, val);
        _root = new_root;
        return erased;
    }

    // Returns an iterator to the specified value in the set if it exists,
    // otherwise returns an iterator to `ordered_set::end`.
    iterator find(const T &val) const {
        return iterator(*this, _find(_root, val));
    }

    // Returns whether the specified value is a member of the set.
    bool contains(const T &val) const {
        return _find(_root, val) != nullptr;
    }

    // Returns an iterator to the value at the specified index in the set.
    // Returns an iterator to `ordered_set::end` if the index is out of bounds.
    iterator get_nth(int index) const noexcept {
        return iterator(*this, _get_nth(_root, index));
    }

    // Returns the number of elements ordered before the specified value in the set.
    int order_of(const T &val) const {
        return _order_of(_root, val);
    }

    // Returns an iterator to the first element in the set
    // that is not ordered before the specified value.
    iterator lower_bound(const T &val) const {
        node *curr = _root, *result = nullptr;
        while (curr) {
            if (!_comp(curr->_val, val)) {
                result = curr;
                curr = curr->_left;
            } else {
                curr = curr->_right;
            }
        }
        return iterator(*this, result);
    }

    // Returns an iterator to the first element in the set
    // that is ordered after the specified value.
    iterator upper_bound(const T &val) const {
        node *curr = _root, *result = nullptr;
        while (curr) {
            if (_comp(val, curr->_val)) {
                result = curr;
                curr = curr->_left;
            } else {
                curr = curr->_right;
            }
        }
        return iterator(*this, result);
    }

    // Removes all elements from the set.
    void clear() noexcept {
        _root = _min_node = _max_node = nullptr;
        _pool.reset();
    }

    // Exchanges the content of the set with another set.
    // This operation invalidates existing iterators for both sets.
    void swap(ordered_set &other) noexcept {
        _pool.swap(other._pool);
        std::swap(_root, other._root);
        std::swap(_min_node, other._min_node);
        std::swap(_max_node, other._max_node);
        std::swap(_comp, other._comp);
        std::swap(_eq, other._eq);
    }

    struct reverse_iterator {
        using value_type = T;
        using reference = const value_type&;
        using pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

      private:
        const ordered_set &_set;
        node *_curr;

      public:
        reverse_iterator(const ordered_set &set, node *node) noexcept : _set(set), _curr(node) {}
        reverse_iterator(const ordered_set &set) noexcept : _set(set), _curr(nullptr) {}

        reference operator*() const {
            assert(_curr);
            return _curr->_val;
        }

        pointer operator->() const {
            assert(_curr);
            return &_curr->_val;
        }

        bool operator==(const reverse_iterator &other) const noexcept {
            return &_set == &other._set && _curr == other._curr;
        }

        bool operator!=(const reverse_iterator &other) const noexcept {
            return !(*this == other);
        }

        reverse_iterator& operator++() {
            assert(_curr);
            if (_curr->_left) {
                _curr = _set._get_max(_curr->_left);
            } else {
                node *par = _curr->_parent;
                while (par && _curr == par->_left) {
                    _curr = par;
                    par = par->_parent;
                }
                _curr = par;
            }
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator result = *this;
            ++*this;
            return result;
        }

        reverse_iterator& operator--() {
            assert(_curr != _set._max_node);
            assert(_curr || !_set.empty());
            if (!_curr) {
                _curr = _set._min_node;
            } else if (_curr->_right) {
                _curr = _set._get_min(_curr->_right);
            } else {
                node *par = _curr->_parent;
                while (par && _curr == par->_left) {
                    _curr = par;
                    par = par->_parent;
                }
                _curr = par;
            }
            return *this;
        }

        reverse_iterator operator--(int) {
            reverse_iterator result = *this;
            --*this;
            return result;
        }

        iterator base() const noexcept {
            node *next = _curr;
            if (!next) return iterator(_set);
            if (next->_right) next = _set._get_min(next->_right);
            else {
                node *par = next->_parent;
                while (par && next == par->_right) {
                    next = par;
                    par = par->_parent;
                }
                next = par;
            }
            return iterator(_set, next);
        }
    };

    // Returns a reverse iterator to the last element in the set.
    reverse_iterator rbegin() const noexcept {
        return reverse_iterator(*this, _max_node);
    }

    // Returns a reverse iterator pointing right before the first element in the set.
    reverse_iterator rend() const noexcept {
        return reverse_iterator(*this);
    }
};

}  // namespace kotone

#endif  // KOTONE_ORDERED_SET_HPP
