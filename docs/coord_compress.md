# Coordinate Compression Hash Map

The `coord_compress_hashmap` is a hash map that maintains a totally ordered set of values $S$ with up to $N$ elements. It supports the following operations for coordinate compression in $\mathcal{O}(1)$ amortized time:

* Insertion
* Deletion
* Index lookup
* Value lookup

The hash map takes $\mathcal{O}(N\log N)$ time to build with lazy evaluation.

The hash map also supports custom comparators and hashes for different data structures.

The time complexity analysis in this documentation assumes that values can be created, compared, and hashed in $\mathcal{O}(1)$ time.

<br>

## Constructor

```cpp
(1) kotone::coord_compress_hashmap<T> m()
(2) kotone::coord_compress_hashmap<T, comp_pred, hash> m()
```

Constructs a hash map for a set $S$ of data type `T`. Initially, $S$ is an empty set.

* (1) Requires `T` to have built-in comparators and hashes. By default, `T` is sorted in ascending order.
* (2) Uses:
    * `comp_pred` for comparing values;
    * `hash` for hashing values.

### Constraints

* `T`, `comp_pred` and `hash` are structs
* `comp_pred` is a strict total order
* `hash` accepts `T` and returns `std::size_t`

### Time complexity

* $\mathcal{O}(1)$

<br>

## Insertion

```cpp
void m.insert(T val)
```

Inserts `val` into $S$.

### Time complexity

* $\mathcal{O}(1)$

<br>

## Deletion

```cpp
void m.erase(T val)
```

Removes `val` from $S$.

### Time complexity

* $\mathcal{O}(1)$

<br>

## Index lookup

```cpp
int m.operator[](const T &val)
```

Returns the compressed index of `val` in $S$.

* If `val` is not a member of $S$, returns $-1$ instead.

### Time complexity

* $\mathcal{O}(N\log N)$ on the first call after $S$ is modified
* $\mathcal{O}(1)$ on subsequent calls

<br>

## Value lookup

```cpp
T m.get_nth(int index)
```

Returns a copy of the value at the specified index in $S$.

### Constraints

* $0\leq$ `index` $\lt N$

### Time complexity

* $\mathcal{O}(N\log N)$ on the first call after $S$ is modified
* $\mathcal{O}(1)$ on subsequent calls

<br>

## Lower bound

```cpp
int m.lower_bound(const T &val)
```

Returns the number of elements in $S$ that are less than `val`.

* Equivalently, returns the index of the first element $v\in S$ for which `comp_pred{}(v, val)` is `false`, or $N$ if no such element exists.

### Time complexity

* $\mathcal{O}(N\log N)$ on the first call after $S$ is modified
* $\mathcal{O}(\log N)$ on subsequent calls

<br>

## Upper bound

```cpp
int m.upper_bound(const T &val)
```

Returns the number of elements in $S$ that are no greater than `val`.

* Equivalently, returns the index of the first element $v\in S$ for which `comp_pred{}(val, v)` is `true`, or $N$ if no such element exists.

### Time complexity

* $\mathcal{O}(N\log N)$ on the first call after $S$ is modified
* $\mathcal{O}(\log N)$ on subsequent calls

<br>

## Size

```cpp
int m.size()
```

Returns $N$, the number of elements in $S$.

### Time complexity

* $\mathcal{O}(N\log N)$ on the first call after $S$ is modified
* $\mathcal{O}(1)$ on subsequent calls

<br>

## Sample code

```cpp
#include <kotone/coord_compress>

int main() {
    kotone::coord_compress_hashmap<int> m;
    assert(m.size() == 0);

    m.insert(10);
    m.insert(100);
    m.insert(-1);
    m.insert(10);

    assert(m.size() == 3);
    assert(m[10] == 1);
    assert(m.get_nth(1) == 10);
    assert(m[40] == -1);
    assert(m.lower_bound(10) == 1);
    assert(m.upper_bound(10) == 2);

    m.insert(40);
    m.erase(-1);
    m.erase(1);

    assert(m.size() == 3);
    assert(m[10] == 0);
    assert(m[40] == 1);
    assert(m[-1] == -1);
}
```

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 168 F - . (Single Dot)](https://atcoder.jp/contests/abc168/tasks/abc168_f)
* [ABC 320 G - Slot Strategy 2 (Hard)](https://atcoder.jp/contests/abc320/tasks/abc320_g)

</details>

<br>
