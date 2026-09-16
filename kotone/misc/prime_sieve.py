# from kotone.misc.prime_sieve import *
# https://github.com/amiast/cpp-utility

M = ...
lpf = list(range(M + 1))
primes = []
for p in range(2, M + 1):
    if lpf[p] == p:
        primes.append(p)
    for q in primes:
        if p * q > M or lpf[p] < q:
            break
        lpf[p * q] = q
