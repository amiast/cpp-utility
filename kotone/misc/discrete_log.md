# Discrete logarithm

## Definition

> **Discrete logarithm**
>
> Given integers $0\leq a,b\lt m$, determine if there exists an integer $x\geq0$ for which $a^x\equiv b\pmod m$. If it exists, find the minimum value of $x$.

## Baby-step giant-step

The baby-step giant-step is a meet-in-the-middle algorithm that solves the discrete logarithm in $O(\sqrt m)$ time.

Suppose $\gcd(a,m)=1$. Let $x=pn-q$ where $n=\Theta(\sqrt m)$ is some predetermined variable. Here, $p$ is the number of **giant steps** and $q$ is the number of **baby steps**. Every $0\leq x\lt m$ can be represented in this form for some $1\leq p\leq\lceil m/n\rceil$ and $0\leq q\leq n$.

The discrete logarithm problem can hence be expressed as finding $p, q$ such that

$$a^{np}\equiv ba^q\pmod m.$$

Let the left-hand side be $f(p)$ and the right-hand side be $g(q)$. A meet-in-the-middle approach is as follows:

- Compute $g(q)$ for all $q$.
- Iterate through $f(p)$ and look for a match.

The algorithm can be extended to cases where $\gcd(a,m)\neq1$. See the implementation below for more details. The extra checks are needed for inputs like $(a,b,m)=(2,0,4)$.

```python
from math import isqrt

def discrete_log(a, b, m):
    """ Returns the minimum `x >= 0` such that `pow(a, x, m) == b % m`.
        If no such `x` exists, returns `-1`.
        Requires `m > 0`.
    """
    assert m > 0
    a, b = a % m, b % m
    n = isqrt(m - 1) + 1
    d = {}
    s = 1 % m
    for q in range(n):
        if s == b:
            return q
        g = s * b % m
        if g not in d:
            d[g] = q
        s = s * a % m
    t = s
    for p in range(1, n + 1):
        if t in d:
            x = p * n - d[t]
            if pow(a, x, m) == b:
                return x
        t = t * s % m
    return -1
```

## Bonus: generalized baby-step giant-step

Given invertible function $f$ and parameters $a, b, m$, the baby-step giant-step algorithm finds the minimum $0\leq x\lt m$ for which $f^x(a)=b$.

By choosing $n=\Theta(\sqrt m)$, the problem can be expressed as finding $x=pn+q$ such that $f^{pn}(a)=f^{-q}(b)$. Note the sign of $q$ is different from the previous section.

Thus, a meet-in-the-middle algorithm is as follows:

- Compute $f^{-q}(b)$ for all $q$.
- Iterate through $f^{pn}(a)$ and look for a match.

The sign in $x=pn+q$ ensures that the $x$ returned is minimum when the baby-step is evaluated in ascending order of $q$.

## Bonus: the set of solutions $\{x\,|\,a^x\equiv b\pmod m\}$

Let $x_0$ be the minimum solution. If $\gcd(a,m)=1$, then the set of solutions is $\{x_0, x_0+\text{ord}_m(a), x_0+2\text{ord}_m(a), \dots\}$ where $\text{ord}_m(a)$ is the multiplicative order of $a$ modulo $m$. See also [`totient.py`](totient.py).

If $\gcd(a,m)\neq1$, then the set of solutions does not follow a simple pattern. For example, the set of solutions to $2^x\equiv4\pmod8$ is $\{2\}$, while the set of solutions to $2^x\equiv0\pmod8$ is $\{3, 4, \dots\}$.

## Related problems & articles

- [cp-algorithms - Discrete Log](https://cp-algorithms.com/algebra/discrete-log.html)
- [Library Checker - Discrete Logarithm](https://judge.yosupo.jp/problem/discrete_logarithm_mod)
- [ARC 042 D - あまり](https://atcoder.jp/contests/arc042/tasks/arc042_d)
- [ABC 270 G - Sequence in mod P](https://atcoder.jp/contests/abc270/tasks/abc270_g) - Discrete log for recurrence
- [ARC 222 F - Triple Transformation](https://atcoder.jp/contests/arc222/tasks/arc222_f) - Discrete log for special function
- [CF 1314 F - Bad Cryptography](https://codeforces.com/contest/1314/problem/F) - Discrete log for nimbers
