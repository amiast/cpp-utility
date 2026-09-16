# from kotone.misc.totient import *
# https://github.com/amiast/cpp-utility

from math import gcd

def totient(n):
    """ Returns phi(n) = n * prod_p (1-1/p). """
    phi = n
    for p in range(2, n + 1):
        if p * p > n:
            break
        if n % p != 0:
            continue
        phi -= phi // p
        while n % p == 0:
            n //= p
    if n > 1:
        phi -= phi // n
    return phi

def mul_order(n, m):
    """ Returns the multiplicative order of n mod m.
        That is, the smallest positive k such that `pow(n, k, m) == 1`.

        If `m < 2 or gcd(n, m) != 1`, returns `-1`.
    """
    if m < 2 or gcd(n, m) != 1:
        return -1
    phi = totient(m)
    k = phi
    pfs = []
    for p in range(2, m + 1):
        if p * p > k:
            break
        if k % p != 0:
            continue
        pfs.append(p)
        while k % p == 0:
            k //= p
    if k > 1:
        pfs.append(k)
    for p in pfs:
        while phi % p == 0 and pow(n, phi // p, m) == 1:
            phi //= p
    return phi
