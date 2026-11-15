# Link/cut tree

The link/cut tree is an online data structure that supports efficient queries on LCA and level ancestors of a dynamic tree. With user customization, the link/cut tree can also support path aggregate queries.

The link/cut tree internally maintains a splay tree, which is only exposed to the user via user-provided callbacks.

See also the `tests` directory for more examples on supporting path aggregate queries.

<br>

## Construction

```cpp
(1) kotone::link_cut_tree<on_update = nullptr, on_reverse = nullptr, on_push = nullptr> lct()
(2) kotone::link_cut_tree<on_update = nullptr, on_reverse = nullptr, on_push = nullptr> lct(int num_vertices)
```

Constructs a link/cut tree with $N$ vertices, where

- (1) $N=0$; or
- (2) $N=$`num_vertices`.

If provided, the following callbacks are applied to the internal splay tree where a vertex `parent` has two children. If the left child exists, `left` is its index. Otherwise, `left` is $-1$. Similarly, `right` is the right child index if it exists, and $-1$ otherwise.

- Calls `void on_update(int parent, int left, int right)` when updating the aggregate of `parent` from its children.
- Calls `void on_reverse(int parent, int left, int right)` when reversing the orientation of `parent`. This is always called after the children are reordered.
- Calls `void on_push(int parent, int left, int right)` when propagating lazy updates from `parent` to its children. This is always called after `on_reverse` is resolved.

### Constraints

- (2) $N\geq0$

### Time complexity

- (1) $\mathcal{O}(1)$
- (2) $\mathcal{O}(N)$

<br>

## Root retrieval

```cpp
int lct.root(int v)
```

Returns the root of the tree containing vertex $v$.

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Cut

```cpp
void lct.cut(int v)
```

Removes the edge between vertex $v$ and its parent.

### Constraints

- $0\leq v\lt N$
- $v$ is not the root of a tree

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Link

```cpp
void lct.link(int v, int p)
```

Adds $p$ as the parent of $v$.

### Constraints

- $0\leq v, p\lt N$
- $v$ is the root of a tree
- $v, p$ are disconnected

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Reorient

```cpp
void lct.reorient(int v)
```

Reorients the tree containing $v$ such that $v$ becomes its root. This operation reverses the path between $v$ and the original root.

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## LCA

```cpp
int lct.lca(int u, int v)
```

If $u$ and $v$ are in the same tree, returns their LCA relative to the root. Otherwise, returns $-1$.

### Constraints

- $0\leq u, v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Depth

```cpp
int lct.depth(int v)
```

Returns the depth of $v$ relative to the root of the tree containing $v$.

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Level ancestor

```cpp
int lct.level_ancestor(int v, int d)
```

Returns the level ancestor of $v$ at depth $d$ relative to the root. In particular, the depth of the root is $0$.

If $d$ is greater than the depth of $v$, returns $-1$.

### Constraints

- $0\leq v\lt N$
- $d\geq0$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>

## Path aggregate

```cpp
void lct.access(int v)
void lct.update(int v)
```

- After calling `lct.access(v)`, the internal splay tree is now rooted at $v$, whose aggregate covers the entire root-to-$v$ path. The lazy update of $v$ is resolved and propagated to its children.
- After calling `lct.update(v)`, the aggregate of $v$ is updated with respect to its children.

To update the value of $v$, the functions should be called in the following order:

```cpp
lct.access(v);
// update values of v
lct.update(v);
```

When the user needs to query the aggregate of a valid $s$-$t$ path or apply lazy propagation to it, the functions should be called in the following order:

```cpp
lct.reorient(s);
lct.access(t);
// retrieve aggregate of v and/or apply lazy update to v
// note: this modifies the structure of the tree
```

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\log N)$

<br>
