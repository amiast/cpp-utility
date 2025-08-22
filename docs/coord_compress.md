# Coordinate Compression Hash Map

The `coord_compress_hashmap` is a hash map that maintains a set of values $S$ with up to $N$ elements. It supports the following operations for coordinate compression in $\mathcal{O}(1)$ time:

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
(2) kotone::coord_compress_hashmap<T, comp_pred, hash, key_eq_pred> m()
```

Constructs a hash map for a set $S$ of data type `T`. Initially, $S$ is an empty set.

* (1) Requires `T` to have built-in comparators and hashes provided by C++.
* (2) Uses:
    * `comp_pred` for sorting values;
    * `hash` for hashing values;
    * `key_eq_pred` for comparing equivalent values.

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
int m.operator[](T val)
```

Returns the (read-only) index of `val` in the ascending order of $S$.

* If provided, the ascending order of $S$ is defined by `comp_pred`.

### Constraints

* `val` is a member of $S$

### Time complexity

* $\mathcal{O}(N\log N)$ for building the hash map
* $\mathcal{O}(1)$ for all subsequent calls before `insert()` and `erase()` are invoked again

<br>

## Value lookup

```cpp
T m.get_nth(int index)
```

Returns a copy of the value at the specified index in the ascending order of $S$.

* If provided, the ascending order of $S$ is defined by `comp_pred`.

### Constraints

* $0\leq$ `index` $\lt |S|$

### Time complexity

* $\mathcal{O}(N\log N)$ for building the hash map
* $\mathcal{O}(1)$ for all subsequent calls before `insert()` and `erase()` are invoked again

<br>

## Size

```cpp
int m.size()
```

Returns $|S|$.

### Time complexity

* $\mathcal{O}(N\log N)$ for building the hash map
* $\mathcal{O}(1)$ for all subsequent calls before `insert()` and `erase()` are invoked again

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

    m.insert(40);
    m.erase(-1);
    m.erase(1);

    assert(m.size() == 3);
    assert(m[10] == 0);
    assert(m[40] == 1);
}
```

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 168 F - . (Single Dot)](https://atcoder.jp/contests/abc168/tasks/abc168_f)
* [ABC 320 G - Slot Strategy 2 (Hard)](https://atcoder.jp/contests/abc320/tasks/abc320_g)

</details>

<br>
