# Retroactive heap

The `retroactive_heap` is a partially retroactive heap that supports efficiently modifying a sequence of operations on a heap and answering queries about the resulting heap.

Given a heap $Q$, an operation $f$ has the effect of one of the following methods:

- $\text{push}(k)$, which pushes an element $k$ onto $Q$;
- $\text{pop}()$, which removes the top element of $Q$ if it exists; and
- $\text{no-op}()$, which does not modify $Q$.

A retroactive heap answers queries about the heap $Q$ that results from applying a sequence of $N$ operations $F=(F_0, \dots, F_{N-1})$ to an empty heap. Moreover, the operations in $F$ may be arbitrarily modified across multiple queries.

Reference: [Erik D. Demaine, John Iacono, and Stefan Langerman. *Retroactive Data Structures.*](https://dl.acm.org/doi/abs/10.1145/1240233.1240236) **[DIL 2007]**

<br>

## Construction

```cpp
(1) kotone::retroactive_heap<T> q(int age)
(2) kotone::retroactive_heap<T, comp_pred> q(int age)
(3) kotone::retroactive_heap<T, comp_pred, on_add, on_del> q(int age)
```

Constructs a retroactive heap $Q$ with $N$ as the specified `age`.

- (1) Creates a min-heap.
- (2), (3) Creates a heap where the top element $m$ satisfies `comp_pred{}(k, m) == false` for all $k\in Q$.
- (3) Uses the following functions to track elements added and deleted from the final $Q$:
    - Calls `on_add(T val)` whenever a modified operation results in adding a copy of `val` to the final $Q$.
    - Calls `on_del(T val)` whenever a modified operation results in deleting a copy of `val` from the final $Q$.

### Constraints

- (1) `T` is comparable using `operator<`.
- (2), (3) `comp_pred` defines a strict weak ordering on `T`.

### Time complexity

- $\Theta(N)$

<br>

## Age

```cpp
int q.age()
```

Returns $N$, the total number of operations.

### Time complexity

- $\Theta(1)$

<br>

## Modify operations

```cpp
(1) void q.set_push(int time, T val)
(2) void q.set_pop(int time)
(3) void q.reset(int time)
```

Modifies an operation $F_t$ where $t$ is the specified `time`.

- (1) Sets $F_t$ to $\text{push}(k)$ where $k$ is the specified `val`.
- (2) Sets $F_t$ to $\text{pop}()$.
- (3) Sets $F_t$ to $\text{no-op}()$.

### Constraints

- $0\leq t\lt N$

### Time complexity

- $\Theta(\log N)$

<br>

## Size

```cpp
int q.size()
```

Returns the number of elements in the final $Q$.

### Time complexity

- $\Theta(1)$

<br>

## Emptiness

```cpp
bool q.empty()
```

Returns `true` if the final $Q$ is empty, otherwise `false`.

### Time complexity

- $\Theta(1)$

<br>

## Top retrieval

```cpp
T q.top()
```

Returns a copy of the top element of the final $Q$.

### Constraints

- The final $Q$ is non-empty.

### Time complexity

- $\Theta(1)$

<br>

## Implementation details

**[DIL 2007]** proves that modifying operations in a retroactive heap is reducible to minimizing $k$ for $k\geq i$ (or maximizing $k$ for $k\leq i$) such that the sum of the first $k$ terms of a special sequence $A$ is $0$ given some index $i$.

In the context of retroactive heaps, $A=(A_0, \dots, A_{N-1})$ is a sequence of $N$ integers such that:

- $|A_i|\leq1$ for $i\in\{0, \dots, N-1\}$;
- the sum of $A$ is $0$; and
- all prefix sums of $A$ are nonnegative.

Given an index $i$, a query either asks for:

- the minimum $r\geq i$ such that the first $r$ terms of $A$ sums to $0$; or
- the maximum $\ell\leq i$ such that the first $\ell$ terms of $A$ sums to $0$.

The queried $r$ and $\ell$ are called *bridges* in **[DIL 2007]**. Both queries are always well-defined by the properties of $A$.

The implementation answers the forward bridge query using binary search over a segment tree that maintains interval sums and minimum prefix sums. By the properties of $A$, the latter query is solved symmetrically with maximum suffix sums instead of minimum prefix sums.

Another non-trivial note is that **[DIL 2007]** assumes all $\text{pop}$ operations only occur when $Q$ is non-empty. By pushing a sufficient number of sentinel values to $Q$ before the first operation $F_0$, any pop that would otherwise occur on an empty heap instead removes a sentinel value. This allows the user to add arbitrary $\text{pop}$ operations and preserves the paper's model.

The implementation manages these sentinels as a single, compressed operation that occurs before $F$ is applied. Since the compressed operation represents multiple sentinel pushes, the corresponding entry of $A$ may exceed $1$ unlike the formulation in **[DIL 2007]**. As this does not violate the other properties of $A$, it preserves the correctness of bridge queries.

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

- [ABC 363 G - Dynamic Scheduling](https://atcoder.jp/contests/abc363/tasks/abc363_g)

</details>

<br>
