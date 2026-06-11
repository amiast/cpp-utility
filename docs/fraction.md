# Fraction

The `fraction` implementation represents a positive rational number $p/q$ as a vertex in the Stern–Brocot tree and maintains its path from the root.

<br>

## Construction

```cpp
(1) kotone::fraction<T> frac()
(2) kotone::fraction<T> frac(T num, T denom)
(3) kotone::fraction<T> frac(std::pair<T, T> frac_pair)
(4) kotone::fraction<T> frac(std::vector<T> rle)
```

Constructs a Stern–Brocot tree vertex representing the specified rational number $p/q$.

`T` is the type of integer values.

- (1) Constructs the root of the Stern–Brocot tree. The stored rational number is $1/1$.
- (2) Constructs the vertex for the specified rational number.
- (3) Constructs the vertex using `first` as the numerator and `second` as the denominator.
- (4) Constructs the vertex from the path in the specified run-length encoding.

### Constraints

- `T` is a `std::signed_integral`
- (2), (3) $p, q\gt0$
- (4) `rle` follows the specification stated in [RLE](#rle)

### Time complexity

- (1) $\Theta(1)$
- (2), (3) $\mathcal{O}(\log\min(p,q))$
- (4) $\Theta(n)$, where $n$ is the length of `rle`

<br>

## Comparison

```cpp
kotone::fraction<int> x, y;
x == y;
x != y;
x < y;
x <= y;
x > y;
x >= y;
```

Supports numeric comparison of positive rational numbers.

### Time complexity

- $\Theta(1)$

<br>

## Value retrieval

```cpp
(1) std::pair<T, T> frac.val()
(2) std::pair<T, T> frac.lower_bound()
(3) std::pair<T, T> frac.upper_bound()
```

Returns the specified value as a coprime numerator-denominator pair.

- (1) Returns the rational number $p/q$.
- (2) Returns the greatest positive rational number that is less than every descendant of $p/q$.
    - If such a value does not exist, returns $0/1$.
- (3) Returns the least positive rational number that is greater than every descendant of $p/q$.
    - If such a value does not exist, returns $1/0$.

### Time complexity

- $\Theta(1)$

<br>

## Depth

```cpp
T frac.depth()
```

Returns the depth of $p/q$ relative to the root.

### Time complexity

- $\Theta(1)$

<br>

## Run-length encoding

```cpp
std::vector<T> frac.rle()
```

Returns the run-length encoding of the path from the root to the current vertex as a sequence of nonzero integers $(d_1, d_2, \dots)$ where each element $d_i$ represents:

- traversal to the $d_i$-th right descendant if $d_i > 0$; or
- traversal to the $(-d_i)$-th left descendant if $d_i < 0$.

### Time complexity

- $\mathcal{O}(\log\min(p,q))$

<br>

## Clear

```cpp
void frac.clear()
```

Resets the vertex to the root of the Stern–Brocot tree and sets the stored rational number to $1/1$.

### Time complexity

- $\mathcal{O}(\log\min(p,q))$

<br>

## Traversal

```cpp
(1) void frac.descend(T steps)
(2) void frac.ascend(T steps)
```

Traverses the Stern–Brocot tree by the specified direction and number of steps $|d|$.

- (1) If $d\gt0$, traverses the right descendants. Otherwise, traverses the left descendants.
- (2) Traverses the ancestors and stops at the root if $d$ is greater than the current steps.

### Constraints

- (2) $d\geq0$

### Time complexity

- (1) Amortized $\Theta(1)$
- (2) $\mathcal{O}(\log\min(p,q))$

<br>

## Lowest common ancestor

```cpp
kotone::fraction<T> frac.lca(kotone::fraction<T> other_frac)
```

Returns the lowest common ancestor of this vertex and `other_frac` in the Stern–Brocot tree.

### Time complexity

- $\mathcal{O}(\log\min(p,q,r,s))$ where $r/s$ is the value of `other_frac`

<br>

## Binary search

```cpp
std::pair<std::pair<T, T>, std::pair<T, T>> kotone::binary_search_rational(auto f, T bound)
```

Given monotone predicate $f$ and integer bound $N$, returns a pair of numerator-denominator pairs for the boundary values of $f$.

- `first` is the greatest positive rational number $a/b$ such that $f(a/b)=$`true` and $a,b\leq N$.
    - If $a/b$ does not exist, `first` is $0/1$.
- `second` is the least positive rational number $c/d$ such that $f(c/d)=$`false` and $c,d\leq N$.
    - If $c/d$ does not exist, `second` is $1/0$.

### Constraints

- `bool f(std::pair<T, T>)` is monotone
- $N\gt0$

### Time complexity

- $\mathcal{O}(T(N)\log^2N)$ where $T(N)$ is the maximum computational cost of $f(p/q)$ for $p,q\leq N$

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

- [ABC 333 G - Nearest Fraction](https://atcoder.jp/contests/abc333/tasks/abc333_g)
- [ABC 408 G - A/B < p/q < C/D](https://atcoder.jp/contests/abc408/tasks/abc408_g)
- [ABC 273 Ex - Inv(0,1)ving Insert(1,0)n](https://atcoder.jp/contests/abc273/tasks/abc273_h)

</details>

<br>
