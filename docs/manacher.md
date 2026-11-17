# Manacher's algorithm

Manacher's algorithm is an algorithm that computes the longest palindrome centered at every position in a string $S$ in $\Theta(|S|)$ time. The actual computation is done on a different string $T$ obtained by placing an extra character `~` between adjacent characters in $S$. In the new string $T$, every odd palindrome corresponds to a palindrome in $S$.

```cpp
std::pair<std::string, std::vector<int>> kotone::manacher(const std::string &s)
```

Returns a pair $(T, R)$ such that

- $T$ is the special string described above, and
- $R$ is a vector of length $|T|$ storing the longest radius of a palindrome centered at every position.

For example, a substring $S_\ell\cdots S_{r-1}$ is a palindrome if and only if $R_{\ell+r-1}\geq r-\ell$.

### Time complexity

- $\Theta(|S|)$

<br>
