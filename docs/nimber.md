# Nimber

The nimbers, also known as Grundy numbers, are known for applications in game theory. When equipped with addition and multiplication, nimbers satisfy a range of properties that make them a suitable candidate for rolling hash.

The nimber sum of two nonnegative integers, denoted $a\oplus b$, has the following properties:
- Equivalence with XOR: $a\oplus b=a\text{ xor } b$
- Associativity: $(a\oplus b)\oplus c=a\oplus(b\oplus c)$
- Commutativity: $a\oplus b=b\oplus a$
- Identity: $a\oplus0=a$
- Inverse: $a\oplus a=0$

The nimber product of two nonnegative integers, denoted $a\otimes b$, has the following properties:
- Associativity: $(a\otimes b)\otimes c=a\otimes(b\otimes c)$
- Commutativity: $a\otimes b=b\otimes a$
- Distributivity: $a\otimes(b\oplus c)=a\otimes b\oplus a\otimes c$
- Identity: $a\otimes1=a$
- Inverse for $a\neq0$

For $n\in\{0, 1, \dots\}$, the set of nimbers less than $2^{2^n}$ form the finite field $\mathbb{F}_{2^{2^n}}$, which leads to applications in rolling hash for problems concerning the XOR of elements in a sequence.

The current implementation supports nimber addition and multiplication for $n=6$.

<br>

## Construction

```cpp
(1) kotone::nimber nim()
(2) kotone::nimber nim(uint64_t val)
```

- (1) Constructs a nimber with default value $0$.
- (2) Constructs a nimber with the specified value.

<br>

## Value retrieval

```cpp
uint64_t nim.val()
```

Returns the raw value as `uint64_t`.

<br>

## Operators

```cpp
kotone::nimber a = 10;
uint64_t b = 20;

a + b;
a * b;

a += b;
a *= b;

a == b;
a != b;

std::cin >> a;
std::cout << a;
```

### Time complexity

- $\Theta\left(\left(\frac{5+\sqrt5}2\right)^n\right)$ for multiplication (see also sequence [A039717](https://oeis.org/A039717) in the OEIS)
- $\Theta(1)$ for other operations

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

- [ABC 274 Ex - XOR Sum of Arrays](https://atcoder.jp/contests/abc274/tasks/abc274_h)

</details>

<br>
