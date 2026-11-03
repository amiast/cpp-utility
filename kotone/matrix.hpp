#ifndef KOTONE_MATRIX_HPP
#define KOTONE_MATRIX_HPP 1

#include <vector>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

namespace matrix {

template <compatible_modint mint> using matrix = std::vector<std::vector<mint>>;

// Returns the identity matrix of the specified dimension.
template <compatible_modint mint> matrix<mint> identity(int dimension) {
    assert(dimension >= 0);
    std::vector result(dimension, std::vector<mint>(dimension));
    for (int i = 0; i < dimension; i++) result[i][i] = 1;
    return result;
}

// Returns the product of two matrices.
// Requires `a` and `b` to be valid matrices.
// Requires the product to exist.
template <compatible_modint mint> matrix<mint> prod(const matrix<mint> &a, const matrix<mint> &b) {
    if (a.empty() && b.empty()) return {};
    assert(!a.empty() && !b.empty() && a[0].size() == b.size());
    int n = a.size(), m = b.size(), l = b[0].size();
    std::vector result(n, std::vector<mint>(l));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            for (int j = 0; j < l; j++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

// Returns the matrix raised to the specified exponent `k`.
// Requires `mat` to be a square matrix.
template <compatible_modint mint> matrix<mint> pow(const matrix<mint> &mat, int64_t k) {
    // assert(k >= 0);
    if (k == 0) return identity<mint>(mat.size());
    if (k == 1) return mat;
    matrix<mint> result = pow(mat, k / 2);
    result = prod(result, result);
    if (k % 2 == 1) result = prod(result, mat);
    return result;
}

// Returns a basis of the specified matrix.
// Requires `mat` to be a valid matrix.
template <compatible_modint mint> matrix<mint> basis(matrix<mint> mat) {
    if (mat.empty()) return {};
    int n = mat.size(), m = mat[0].size();
    int r = 0;
    for (int k = 0, j = 0; k < n && j < m; k++, j++, r++) {
        while (j < m) {
            for (int i = k; i < n; i++) {
                if (mat[i][j] != 0) {
                    std::swap(mat[k], mat[i]);
                    break;
                }
            }
            if (mat[k][j] != 0) break;
            j++;
        }
        if (j == m) break;
        mint inv = mat[k][j].inv();
        for (int nj = 0; nj < m; nj++) mat[k][nj] *= inv;
        for (int i = k + 1; i < n; i++) {
            for (int nj = m - 1; nj >= j; nj--) mat[i][nj] -= mat[i][j] * mat[k][nj];
        }
    }
    mat.resize(r);
    return mat;
}

// Returns the determinant specified matrix.
// Requires `mat` to be a square matrix.
template <compatible_modint mint> mint det(matrix<mint> mat) {
    int n = mat.size();
    mint result = 1;
    for (int k = 0; k < n; k++) {
        for (int i = k; i < n; i++) {
            if (mat[i][k] == 0) continue;
            if (i != k) {
                std::swap(mat[i], mat[k]);
                result = -result;
            }
            break;
        }
        if (mat[k][k] == 0) return 0;
        result *= mat[k][k];
        mint inv = mat[k][k].inv();
        for (int j = 0; j < n; j++) mat[k][j] *= inv;
        for (int i = k + 1; i < n; i++) {
            for (int j = n - 1; j >= k; j--) mat[i][j] -= mat[i][k] * mat[k][j];
        }
    }
    return result;
}

// Returns a pair `{inv, found}` representing the inverse of the determinant such that
// - `inv` is the inverse matrix and `found == true` if the inverse exists, or
// - `inv` is empty and `found == false` if the inverse does not exist.
//
// Requires `mat` to be a square matrix.
template <compatible_modint mint> std::pair<matrix<mint>, bool> inv(matrix<mint> mat) {
    int n = mat.size();
    for (int i = 0; i < n; i++) {
        mat[i].resize(n * 2);
        mat[i][i + n] = 1;
    }
    for (int k = 0; k < n; k++) {
        for (int i = k; i < n; i++) {
            if (mat[i][k] == 0) continue;
            if (i != k) std::swap(mat[i], mat[k]);
            break;
        }
        if (mat[k][k] == 0) return {{}, false};
        mint inv = mat[k][k].inv();
        for (int j = 0; j < n * 2; j++) mat[k][j] *= inv;
        for (int i = 0; i < n; i++) {
            if (i == k) continue;
            for (int j = n * 2 - 1; j >= k; j--) mat[i][j] -= mat[i][k] * mat[k][j];
        }
    }
    for (int i = 0; i < n; i++) mat[i].erase(mat[i].begin(), mat[i].begin() + n);
    return {mat, true};
}

}  // namespace matrix

}  // namespace kotone

#endif  // KOTONE_MATRIX_HPP
