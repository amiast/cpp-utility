# Monotonic deque

The `monotonic_deque` is a deque that maintains the minimum or maximum value of an implicit queue $Q$. Its application is also known as the sliding window technique.

<br>

## Constructor

```cpp
(1) kotone::monotonic_deque<T> md()
(2) kotone::monotonic_deque<T, comp_pred> md()
```

Constructs a deque for an empty queue $Q$ of data type `T`.

* (1) The deque maintains $\min Q$.
* (2) The deque maintains a minimal element of $Q$ under `comp_pred`.
    - Corresponds to $\min Q$ when using `std::less<T>` as `comp_pred`.
    - Corresponds to $\max Q$ when using `std::greater<T>` as `comp_pred`.

### Constraints

* (1) `T` supports comparison via `operator<`
* (2) `comp_pred` is a strict total order of `T`

### Time complexity

* $\mathcal{O}(1)$

<br>

## Push

```cpp
void md.push(T val)
```

Pushes the specified value to the end of $Q$.

### Time complexity

* Amortized $\mathcal{O}(1)$

<br>

## Pop

```cpp
void md.pop()
```

Pops the element at the start of $Q$.

### Constraints

* $|Q|\gt0$

### Time complexity

* Amortized $\mathcal{O}(1)$

<br>

## Query

```cpp
T md.query()
```

Returns a copy of the minimal or maximal element of $Q$ as specified in [constructor](#constructor).

### Constraints

* $|Q|\gt0$

### Time complexity

* $\mathcal{O}(1)$

<br>

## Size

```cpp
int md.size()
```

Returns $|Q|$.

### Time complexity

* $\mathcal{O}(1)$

<br>

## Emptiness

```cpp
bool md.empty()
```

Returns `true` if $Q$ is empty, otherwise `false`.

### Time complexity

* $\mathcal{O}(1)$

<br>
