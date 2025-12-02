# Segment Tree

The `segment_tree` is a data structure that maintains a sequence of monoids $A$ indexed over an interval $[L, R)$. It is capable of performing the following operations in $\mathcal{O}(N)$ time, where $N=R-L$:

* Reassigning a value in $A$
* Computing the product of elements in $A$ over a subinterval of $[L, R)$

This implementation closely follows the AtCoder Library's naming conventions. See also [AtCoder Library's documentation on segment tree](https://atcoder.github.io/ac-library/production/document_en/segtree.html) for tips on using the segment tree.

Whereas the AtCoder Library's implementation directly stores $\mathcal{O}(N)$ nodes, this version only stores modified nodes, making it suitable for sparse data over a large interval.

This implementation uses user-defined functions for monoid-related operations and assumes these functions run in $\mathcal{O}(1)$ time.

<br>

## Constructor

```cpp
(1) kotone::segment_tree<S, op, e> seg(int64_t low, int64_t high)
(2) kotone::segment_tree<S, op, e> seg(int64_t length)
```

Constructs a segment tree for the specified interval $[L, R)$.

* All values are initialized as `e()`.
* (2) is equivalent to constructing a segment tree for the interval $[0, N)$.

### Constraints

* `S op(S a, S b)` is associative
* `S e()` returns the identity element of `S`
* (1) $L\leq R$
* (2) $N\geq0$

### Time complexity

* $\mathcal{O}(1)$

<br>

## Assignment

```cpp
void seg.set(int64_t pos, S val)
```

Reassigns the value at the specified position $p$.

### Constraints

* $L\leq p\lt R$

### Time complexity

* $\mathcal{O}(\log N)$

<br>

## Product

```cpp
(1) S seg.prod(int64_t low, int64_t high)
(2) S seg.all_prod()
```

Returns the product of the values over the specified subinterval.

* (1) Returns the product of $A_\ell, A_{\ell+1}, \dots, A_{r-1}$. If $\ell=r$, returns `e()`.
* (2) Returns the product of $A_L, A_{L+1}, \dots, A_{R-1}$. If $L=R$, returns `e()`.

### Constraints

* (1) $L\leq\ell\leq r\leq R$

### Time complexity

* (1) $\mathcal{O}(\log N)$
* (2) $\mathcal{O}(1)$

<br>

## Binary search

```cpp
(1) int64_t seg.max_right(int64_t low, G g)
(2) int64_t seg.min_left(int64_t high, G g)
```

Performs binary search on $A$ using the specified monotonic predicate $g$.

* (1) Returns the maximum `high`$\leq R$ such that `g(seg.prod(low, high)) == true`.
* (2) Returns the minimum `low`$\geq L$ such that `g(seg.prod(low, high)) == true`.

### Constraints

* `bool g(S x)` is monotonic
* `g(e()) == true`
* (1) $L\leq\ell\leq R$
* (2) $L\leq r\leq R$

### Time complexity

* $\mathcal{O}(\log N)$

<br>
