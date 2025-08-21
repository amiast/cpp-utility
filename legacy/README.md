# cpp-utility
A collection of useful C++ data structures for competitive programming.

## Table of Contents

- [`binary_indexed_tree.h`](#binary_indexed_treeh)
- [`bucket_sort.h`](#bucket_sorth)
- [`floyd_warshall.h`](#floyd_warshallh)
- [`prime.h`](#primeh)
- [`segment_tree.h`](#segment_treeh)
- [`union_find.h`](#union_findh)

## `binary_indexed_tree.h`

A `BITree` stores prefix sums of an array of numbers.

Initially, a `BITree<int>(n)` represents an array of `n` numbers all set to `0`.
Values can be incremented using `update(index, value)`.
Use `query(index)` to find the sum of the `index`-length prefix of the array.
In this implementation, indices start from `1`.

## `bucket_sort.h`

A sorting algorithm that efficiently sorts an array of numbers. If the input is distributed uniformly (and randomly), the algorithm's expected time complexity is linear and runs faster than `std::sort` on large inputs ($n \geq 500$ for the current implementation).

```
bucket_sort(vector);
```

## `floyd_warshall.h`

hi

## `prime.h`

The `sieve(n)` function returns a `std::vector` of all prime numbers at most `n`.

```
std::vector<int> primes = sieve(100);
primes.at(0); // 2
primes.at(1); // 3
primes.at(2); // 5
...
primes.at(24); // 97
```

## `segment_tree.h`

A `SegmentTree` holds information about elements in an interval of an array.
It can answer **range queries** like "What is the sum of the numbers between the 1000th and 2000th elements?" or "What is the greatest number between the 9000th and 19000th elements?"

### Usage

The following code creates a segment tree from a `std::vector` containing values of type `T`. Each query is responded with some value of type `U`.
The function `operation` specifies how the responses for two adjacent intervals should be combined to return the response for the union of the intervals.
The function `evaluate` specifies the response for an interval containing exactly one value.

```
SegmentTree<T, U> seg_tree(vector, operation, evaluate);
```

If result type `U` is identical to the data type `T` of values in the vector, the following is also allowed:

```
SegmentTree<T> seg_tree(vector, operation, evaluate);
```

To make a range query for interval `[l, r]` (inclusive), use

```
seg_tree.query_range(l, r);
```

To update the value at `index` to `val`, use

```
seg_tree.update(index, val);
```

Note that this method does not modify the original `vector`. Modifying the original list of values after the creation of a `SegmentTree` also does not modify the tree.

### Examples

The following code snippet uses `SegmentTree<int>` to store sums of intervals of a list of `int`'s:

```
// Given the sums of two intervals, return the sum of their union
int sum(int x, int y) {
    return x + y;
}

// The sum of a number is the number itself
int evaluate(int x) {
    return x;
}

int main() {
    std::vector<int> v = {10, 20, 40, 80, 160};
    SegmentTree<int> seg_tree(v, sum, evaluate);

    seg_tree.query_range(0, 2); // 70

    seg_tree.update(2, 10);
    seg_tree.query_range(0, 2); // 40
}
```

By using a different `evaluate` function, the above code can be modified to store the number of appearances of the number `42` in an interval.

```
int sum(int x, int y) {
    return x + y;
}

// Return 1 if x == 42, otherwise 0
int is_42(int x) {
    return x == 42;
}

int main() {
    std::vector<int> v = {42, 41, 42, 43, 42, 42};
    SegmentTree<int> seg_tree(v, sum, is_42);

    seg_tree.query_range(2, 5); // 3

    seg_tree.update(3, 42);
    seg_tree.query_range(2, 5); // 4
}
```

[AtCoder Beginner Contest 343 | F - Second Largest Query](https://atcoder.jp/contests/abc343/tasks/abc343_f?lang=en) can be solved using `SegmentTree` to find **the number of occurrences of the second largest number in an interval**. A complete solution implementing this can be found [here](https://atcoder.jp/contests/abc343/submissions/60488868). (Note: undocumented code.)

## `union_find.h`

A `UnionFind` data structure stores a **partition** of a set. It supports two main operations: `update_union(val_1, val_2)` merges the two sets containing `val_1` and `val_2` into their union, and `find(val)` returns a representative member of the set containing `val`.

### Examples

[AtCoder Beginner Contest 383 | E - Sum of Max Matching](https://atcoder.jp/contests/abc383/tasks/abc383_e?lang=en) is a graph problem that can be solved using a clever greedy algorithm that utilizes `UnionFind`.
