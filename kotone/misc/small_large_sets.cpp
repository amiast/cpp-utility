// #include <kotone/misc/small_large_sets.cpp>
// https://github.com/amiast/cpp-utility

#include <set>

// A generic class that monitors partition into two multisets.
// The default invariant is `small.size() >= large.size()`.
// Additional operations should be inserted under tags `ADD_SMALL`, 'DEL_SMALL`, `ADD_LARGE` and `DEL_LARGE`.
template <typename T> struct small_large_sets {
  public:
    std::multiset<T, std::greater<>> small;
    std::multiset<T> large;

  private:
    void _balance() {
        while (small.size() && small.size() > large.size() + 1) {  // modify invariant here
            // ADD_LARGE
            large.insert(*small.begin());
            // DEL_SMALL
            small.erase(small.begin());
        }
        while (large.size() && small.size() < large.size()) {  // modify invariant here
            // ADD_SMALL
            small.insert(*large.begin());
            // DEL_LARGE
            large.erase(large.begin());
        }
        while (small.size() && large.size() && *small.begin() > *large.begin()) {
            T s = *small.begin();
            T l = *large.begin();
            // DEL_SMALL
            small.erase(small.begin());
            // DEL_LARGE
            large.erase(large.begin());
            // ADD_SMALL
            small.insert(l);
            // ADD_LARGE
            large.insert(s);
        }
    }

  public:
    void add(T x) {
        // ADD_SMALL
        small.insert(x);
        _balance();
    }

    void del(T x) {
        if (small.size() && x <= *small.begin()) {
            // DEL_SMALL
            small.erase(small.find(x));
        } else {
            // DEL_LARGE
            large.erase(large.find(x));
        }
        _balance();
    }
};
