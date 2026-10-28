# Möbius inversion formula

This article discusses techniques for combinatorial problems related to divisors and multiples.

## Möbius function

Given positive integer $n$, the **Möbius function** $\mu(n)$ is defined as

- $\mu(1)=1$;
- $\mu(n)=0$ if $n$ is divisible by a square greater than $1$;
- $\mu(n)=(-1)^k$ if $n$ is square-free and divisible by $k$ distinct primes.

The following python snippet computes $\mu$ via the sieve of Eratosthenes in $\mathcal{O}(M\log\log M)$ time.

```python
M = ...
is_prime = [True] * M
moebius = [1] * M
for p in range(2, M):
    if not is_prime[p]:
        continue
    for q in range(p * p, M, p):
        is_prime[q] = False
    for q in range(p, M, p):
        moebius[q] = -moebius[q]
    for q in range(p * p, M, p * p):
        moebius[q] = 0
```

### Properties

- $\mu$ is multiplicative: $\mu(ab)=\mu(a)\mu(b)$ for coprime $a, b$.
- The Dirichlet inverse of $\mu$ is the constant function $\mathbf{1}(n)=1$.
    - Equivalently, for $n\geq2$, $\sum_{d|n}\mu(d)=0$.
    - $\mu$ also appears in the Dirichlet inverse of many important functions, as seen in later sections and in related problems. See also [Wikipedia - Dirichlet inverse](https://en.wikipedia.org/wiki/Dirichlet_convolution#Dirichlet_inverse).

## Möbius inversion formula

A function is said to be **arithmetic** if its domain is the positive integers and its range is a subset of the complex numbers.

Consider two arithmetic functions $f, F$ satisfying

$$F(n)=\sum_{d|n}f(d).$$

The **Möbius inversion formula** states that

$$f(n)=\sum_{d|n}F(d)\,\mu\left(\frac{n}{d}\right).$$

### As a Dirichlet convolution

The Möbius inversion formula can also be stated in terms of **Dirichlet convolution**. The Dirichlet convolution of sequences $A, B$ is defined as the sequence $C=A\ast B$ such that

$$C_k=\sum_{ij=k}A_iB_j,$$

so $F=f\ast\mathbf{1}$ and $f=F\ast\mu$. For details on Dirichlet convolution, see [`convolution.md`](convolution.md).

In general, if $\alpha$ is an arithmetic function with Dirichlet inverse $\alpha^{-1}$, then the inversion formula states that $F=f\ast\alpha$ if and only if $f=F\ast\alpha^{-1}$.

Since $\mathbf{1}$ and $\mu$ are multiplicative, the Dirichlet convolution can be performed rather efficiently. In fact, $f=F\ast\mu$ can be computed just as efficiently without $\mu$ at all! The divisor Möbius transform, discussed in a later section, computes $f(1), \dots, f(N)$ from $F(1), \dots, F(N)$ in $\mathcal{O}(N\log\log N)$ time.

This formulation is useful in problems involving the sum of functions of GCD, such as computing $\sum_{a,b\in S}f(\gcd(a,b))g(a)h(b)$ for some collection $S$ of positive integers.

### Sparse computation for a single $f(N)$

To compute $f(N)$ from $F$, we iteratively recover

$$f(n)=F(n)-\sum_{\substack{d|n\\ d\lt n}}f(d)$$

in ascending order of divisors $n$ of $N$ in $\mathcal{O}(\sqrt N)$ time, or $\mathcal{O}(\sigma_0(N)^2)$ time with precomputation. Here, the divisor function $\sigma_0(N)$ denotes the number of positive divisors of $N$.

### Worked example: [ABC 304 F - Shift Table](https://atcoder.jp/contests/abc304/tasks/abc304_f)

Let $F(n)$ be the number of valid schedules with period $n$, and let $f(n)$ be the number of valid schedules with *minimum* period $n$. Then

$$F(n)=\sum_{d|n}f(d).$$

We enumerate $F(n)$ in $\mathcal{O}(N)$ time for each divisor $n$ of $N$, then use the inversion formula to compute $F(N)-f(N)$ for the answer.

### Generalization: abelian groups

The Möbius inversion formula works with all abelian groups. For example, if $f, F$ are related by

$$F(n)=\prod_{d|n}f(d),$$

then

$$f(n)=\prod_{d|n}F(d)^{\mu(n/d)}.$$

### Generalization: non-arithmetic functions

We previously required $f, F$ to be arithmetic functions. However, the inversion formula works for complex-valued functions in general.

The general Möbius inversion formula states that if $f,F$ are defined on the domain $[1, \infty)$ and satisfy

$$F(x)=\sum_{1\leq n\leq x}f\left(\frac{x}{n}\right)\alpha(n),$$

then

$$f(x)=\sum_{1\leq n\leq x}F\left(\frac{x}{n}\right)\alpha^{-1}(n).$$

Here, the summations are over positive integers $n$ no greater than $x$.

## GCD convolution

In the previous sections, we defined $F(n)$ as the sum over divisors. Now consider a different setting in which $F(d)$ is the sum over multiples $n$ of $d$ instead:

$$F(d)=\sum_{d|n}f(n).$$

Its inversion is

$$f(d)=\sum_{d|n}F(n)\,\mu\left(\frac{n}{d}\right),$$

and if $f(n)=0$ for $n\gt N$, the inversion formula can be restated as

$$f(i)=\sum_{j=1}^{\lfloor N/i\rfloor}F(ij)\mu(j).$$

This implies an efficient algorithm for computing $F$ from $f$ and vice versa. The **multiple zeta transform** (analogous to superset zeta transform) computes $F(1), \dots, F(N)$ from $f$, whereas the **multiple Möbius transform** (analogous to superset Möbius transform) computes $f(1), \dots, f(N)$ from $F$. The time complexity of the following implementation is $\mathcal{O}(N\log N)$.

```python
def multiple_zeta_transform_slow(f):
    M = len(f)
    for i in range(1, M):
        for j in range(2, (M - 1) // i + 1):
            f[i] += f[i * j]

def multiple_moebius_transform_slow(F):
    M = len(F)
    for i in range(M - 1, 0, -1):
        for j in range(2, (M - 1) // i + 1):
            F[i] -= F[i * j]
```

With the sieve of Eratosthenes, both transforms can be computed more efficiently in $\mathcal{O}(N\log\log N)$ time.

```python
is_prime = [...]

def multiple_zeta_transform(f):
    M = len(f)
    for p in range(2, M):
        if not is_prime[p]:
            continue
        for k in range((M - 1) // p, 0, -1):
            f[k] += f[p * k]

def multiple_moebius_transform(F):
    M = len(F)
    for p in range(2, M):
        if not is_prime[p]:
            continue
        for k in range(1, (M - 1) // p + 1):
            F[k] -= F[p * k]
```

Finally, these transforms are used in **GCD convolution**. Given two sequences $A, B$, the GCD convolution computes the sequence $G$ defined as

$$G_k=\sum_{\gcd(i, j)=k}A_iB_j.$$

```python
def gcd_convolution(A, B):
    assert len(A) == len(B)
    a = A[::]
    b = B[::]
    multiple_zeta_transform(a)
    multiple_zeta_transform(b)
    C = [a[i] * b[i] for i in range(len(A))]
    multiple_moebius_transform(C)
    return C
```

## LCM convolution

For the original definition of $f, F$ over divisors, we can likewise define **divisor zeta transform** (analogous to subset zeta transform) and **divisor Möbius transform** (analogous to subset Möbius transform).

As stated in an earlier section, the divisor zeta transform computes the Dirichlet convolution with $\mathbf{1}$, whereas the divisor Möbius transform computes the Dirichlet convolution with $\mu$.

```python
def divisor_zeta_transform_slow(f):
    M = len(f)
    for i in range(M - 1, 0, -1):
        for j in range(2, (M - 1) // i + 1):
            f[i * j] += f[i]

def divisor_moebius_transform_slow(F):
    M = len(F)
    for i in range(1, M):
        for j in range(2, (M - 1) // i + 1):
            F[i * j] -= F[i]
```

Again, we can use the sieve of Eratosthenes for better time complexity.

```python
is_prime = [...]

def divisor_zeta_transform(f):
    M = len(f)
    for p in range(2, M):
        if not is_prime[p]:
            continue
        for k in range(1, (M - 1) // p + 1):
            f[p * k] += f[k]

def divisor_moebius_transform(F):
    M = len(F)
    for p in range(2, M):
        if not is_prime[p]:
            continue
        for k in range((M - 1) // p, 0, -1):
            F[p * k] -= F[k]
```

Lastly, these transforms are used in the **LCM convolution**, which computes the sequence $L$ such that

$$L_k=\sum_{\text{lcm}(i, j)=k}A_iB_j.$$

```python
def lcm_convolution(A, B):
    assert len(A) == len(B)
    a = A[::]
    b = B[::]
    divisor_zeta_transform(a)
    divisor_zeta_transform(b)
    C = [a[i] * b[i] for i in range(len(A))]
    divisor_moebius_transform(C)
    return C
```

## Related problems

- [noshi91のメモ - 高速ゼータ変換の約数版 O(N log(log(N)))](https://noshi91.hatenablog.com/entry/2018/12/27/121649)
- [Library Checker - Gcd Convolution](https://judge.yosupo.jp/problem/gcd_convolution)
- [Library Checker - Lcm Convolution](https://judge.yosupo.jp/problem/lcm_convolution)
- [ABC 020 D - LCM Rush](https://atcoder.jp/contests/abc020/tasks/abc020_d) - sparse inversion over multiples
- [ARC 064 F - Rotated Palindromes](https://atcoder.jp/contests/arc064/tasks/arc064_d) - sparse inversion over divisors
- [「みんなのプロコン 2018」決勝 A - Uncommon](https://atcoder.jp/contests/yahoo-procon2018-final/tasks/yahoo_procon2018_final_a) - inversion formula
- [九州大学プログラミングコンテスト2018 I - Buffalo](https://atcoder.jp/contests/qupc2018/tasks/qupc2018_i) - multiple Möbius transform
- [AGC 039 C - Division by Two with Something](https://atcoder.jp/contests/agc039/tasks/agc039_c) - divisor Möbius transform
- [ABC 162 E - Sum of gcd of Tuples (Hard)](https://atcoder.jp/contests/abc162/tasks/abc162_e) - $N$-fold GCD convolution
- [ABC 170 D - Not Divisible](https://atcoder.jp/contests/abc170/tasks/abc170_d) - divisor zeta transform
- [ABC 206 E - Divide Both](https://atcoder.jp/contests/abc206/tasks/abc206_e) - GCD convolution
- [ABC 304 F - Shift Table](https://atcoder.jp/contests/abc304/tasks/abc304_f) - inversion formula
- [ABC 349 F - Subsequence LCM](https://atcoder.jp/contests/abc349/tasks/abc349_f) - sparse LCM convolution
- [NDPC L - LCM](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_l) - online computation of $c(j)=A_j\sum_{i\lt j}c(i)A_i/\gcd(A_i,A_j)$ via inversion of $1/n=\sum_{d|n}f(d)$
    - [AGC 038 C - LCMs](https://atcoder.jp/contests/agc038/tasks/agc038_c) - $F(n)=1/n=\sum_{d|n}f(d)$ (the resulting sequence $n\cdot f(n)$ is [A023900](https://oeis.org/A023900) in the OEIS); also possible with offline GCD convolution in $\mathcal{O}(M\log\log M)$ time where $M=\max A_i$
    - [ARC 185 E - Adjacent GCD](https://atcoder.jp/contests/arc185/tasks/arc185_e) - $F(n)=n=\sum_{d|n}\varphi(d)$ gives Euler's totient function since $\text{id}=\varphi\ast\mathbf{1}$
- [ARC 233 B - Count of Euclid Function Call](https://atcoder.jp/contests/arc233/tasks/arc233_b) - general inversion formula for non-arithmetic function $f(x)=|\lbrace (a,b)|1\leq a\lt b, an+bm\leq x,\gcd(a,b)=1\rbrace|$

## Test yourself

- What does the following python snippet print?

```python
A = [0] * 11
A[1] = 1
divisor_moebius_transform(A)
print(A[1:])
```

<details><summary>Answer</summary>

Since `A` stores the unit function $\varepsilon$ of Dirichlet convolution, the divisor Möbius transform computes $\varepsilon\ast\mu=\mu$ in place. The above code prints $\mu(1), \mu(2), \dots, \mu(10)$, which are `[1, -1, -1, 0, -1, 1, -1, 0, 0, 1]`.

</details>
