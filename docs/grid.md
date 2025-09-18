# Grid

A $H\times W$ grid is a collection of $HW$ cells organized into $H$ rows and $W$ columns. Each cell can be identified by a unique integer pair $(i, j)$ for $0\leq i\lt H$ and $0\leq j\lt W$. Since a $H\times W$ grid is equivalent to a graph consisting of $HW$ vertices, many grid-related problems can be reduced to graph problems.

In most grid-related problems, edges only exist between adjacent cells. Say cells $(i, j)$ and $(i^\prime, j^\prime)$ are orthogonal neighbors if and only if their Manhattan distance $|i-i^\prime|+|j-j^\prime|$ is $1$. This implementation simply refers to orthogonal neighbors as "neighbors".

Less commonly, edges can exist between both orthogonal and diagonal cells. Say cells $(i, j)$ and $(i^\prime, j^\prime)$ are orthogonal or diagonal neighbors if and only if their Chebyshev distance $\max\{|i-i^\prime|, |j-j^\prime|\}$ is $1$. Since there are at most eight neighboring cells $(i^\prime, j^\prime)$ for each $(i, j)$, this implementation refers to such neighbors as "eight-neighbors".

The `grid` struct provides utilities for identifying neighboring cells in a grid.

<br>

## Constructor

```cpp
kotone::grid<T = int> g(T num_rows, T num_cols)
```

Constructs a $H\times W$ grid where $H$ is `num_rows` and $W$ is `num_cols`.

### Constraints

* $H\geq0$
* $W\geq0$
* `T` is an integral type

### Time complexity

* $\mathcal{O}(1)$

<br>

## Cell membership

```cpp
bool g.contains(T row, T col)
```

Returns whether the grid contains cell $(i, j)$ specified by the given coordinates.

* A $H\times W$ grid contains $(i, j)$ if and only if $0\leq i\lt H$ and $0\leq j\lt W$.

### Time complexity

* $\mathcal{O}(1)$

<br>

## Neighbor iteration

```cpp
struct grid<T>::neighbors {
    neighbors::iterator begin(), end();
};

neighbors g.neighbors(T row, T col)
```

Returns a `neighbors` struct that supports iterators over the neighbors of cell $(i, j)$ specified by the given coordinates.

* For each valid neighbor, the iterator points to a read-only `std::pair<int, int>` representing its coordinates.
* The order of traversal is undefined.
* See [sample code](#sample-code) for details.

### Constraints

* $0\leq i\lt H$
* $0\leq j\lt W$

### Time complexity

* Construction in $\mathcal{O}(1)$ time
* Full iteration in $\mathcal{O}(1)$ time

<br>

## Eight-neighbor iteration

```cpp
struct grid<T>::eight_neighbors {
    eight_neighbors::iterator begin(), end();
};

eight_neighbors g.eight_neighbors(T row, T col)
```

Returns a `eight_neighbors` struct that supports iterators over the eight-neighbors of cell $(i, j)$ specified by the given coordinates.

* For each valid eight-neighbor, the iterator points to a read-only `std::pair<int, int>` representing its coordinates.
* The order of traversal is undefined.
* See [sample code](#sample-code) for details.

### Constraints

* $0\leq i\lt H$
* $0\leq j\lt W$

### Time complexity

* Construction in $\mathcal{O}(1)$ time
* Full iteration in $\mathcal{O}(1)$ time

<br>

## Sample code

```cpp
#include <string>
#include <vector>
#include <kotone/grid>

int main() {
    kotone::grid g(3, 4);
    std::vector<std::string> s{
        "..#.",
        "##..",
        "..##"
    };

    assert(g.contains(0, 0));
    assert(g.contains(2, 1));
    assert(!g.contains(3, 4));
    assert(!g.contains(-1, 1));

    int num_neighbors = 0;
    for (auto [ni, nj] : g.neighbors(1, 0)) num_neighbors++;
    assert(num_neighbors == 3);

    int num_eight_neighbors = 0;
    for (auto [ni, nj] : g.eight_neighbors(1, 0)) num_eight_neighbors++;
    assert(num_eight_neighbors == 5);

    for (auto [ni, nj] : g.neighbors(1, 2)) {
        if (s[ni][nj] == '#') continue;
        assert(ni == 1);
        assert(nj == 3);
    }
}
```

<br>
