# Subset transform

This article discusses fast zeta transform and Möbius transform, which run in $\mathcal{O}(n2^n)$ time.

See also: [AtCoder Algorithm Lectures - ゼータ・メビウス変換（部分集合関係）](https://info.atcoder.jp/entry/algorithm_lectures/subset_zeta_mobius)

## Subset zeta transform

Given a sequence $A=(A_0, \dots, A_{2^n-1})$, the subset zeta transform computes the sequence $B=(B_0, \dots, B_{2^n-1})$ given by

$$B_T=\sum_{S\subseteq T}A_S.$$

```python
def subset_zeta_transform(A, n):
    assert len(A) == 1 << n
    B = A[:]
    for i in range(n):
        for k in range(1 << n):
            if k >> i & 1:
                B[k] += B[k ^ 1 << i]
    return B
```

### Related problems

- [ARC 100 E - Or Plus Max](https://atcoder.jp/contests/arc100/tasks/arc100_c)
- [ARC 106 E - Medals](https://atcoder.jp/contests/arc106/tasks/arc106_e)
- [NDPC M - Numeral](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_m)

The subset zeta transform is analogous to computing multidimensional prefix sums over a $2\times2\times\dots\times2$ array.

- In [ARC 136 D - Without Carry](https://atcoder.jp/contests/arc136/tasks/arc136_d), the editorial remarks that the fast zeta transform can be similarly done in bases other than $2$.
- [ABC 465 F - Sjeltzer?](https://atcoder.jp/contests/abc465/tasks/abc465_f)

## Subset Möbius transform

Subset Möbius transform is the inverse operation of the subset zeta transform. More precisely, given a sequence $B=(B_0, \dots, B_{2^n-1})$, the subset Möbius transform computes the sequence $A=(A_0, \dots, A_{2^n-1})$ given by

$$B_T=\sum_{S\subseteq T}A_S.$$

Equivalently,

$$A_T=\sum_{S\subseteq T}(-1)^{|T\setminus S|}B_S.$$

```python
def subset_moebius_transform(B, n):
    assert len(B) == 1 << n
    A = B[:]
    for i in range(n):
        for k in range(1 << n):
            if k >> i & 1:
                A[k] -= A[k ^ 1 << i]
    return A
```

## Superset transform

Superset zeta and Möbius transforms can be similarly defined for the following relationship:

$$B_T=\sum_{T\subseteq S}A_S$$

and

$$A_T=\sum_{T\subseteq S}(-1)^{|S\setminus T|}B_S.$$

```python
def superset_zeta_transform(A, n):
    assert len(A) == 1 << n
    B = A[:]
    for i in range(n):
        for k in range(1 << n):
            if ~k >> i & 1:
                B[k] += B[k | 1 << i]
    return B

def superset_moebius_transform(B, n):
    assert len(B) == 1 << n
    A = B[:]
    for i in range(n):
        for k in range(1 << n):
            if ~k >> i & 1:
                A[k] -= A[k | 1 << i]
    return A
```

### Related problems

- [JOI 2018 Final Round E - Snake Escaping](https://atcoder.jp/contests/joi2018ho/tasks/joi2018ho_e)
- [ABC 423 F - Loud Cicada](https://atcoder.jp/contests/abc423/tasks/abc423_f)
