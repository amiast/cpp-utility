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
                if (mat[i][k] == 0) continue;
                if (i != k) std::swap(mat[i], mat[k]);
                break;
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

// Returns a pair `{inv, found}` representing the inverse of the matrix such that
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

// Returns the upper Hessenberg form of the specified matrix.
// Requires `mat` to be a square matrix.
// Reference: https://judge.yosupo.jp/problem/characteristic_polynomial
template <compatible_modint mint> matrix<mint> upper_hessenberg(matrix<mint> mat) {
    int n = mat.size();
    for (int k = 0; k < n - 2; k++) {
        for (int i = k + 1; i < n; i++) {
            if (mat[i][k] == 0) continue;
            if (i != k + 1) {
                std::swap(mat[i], mat[k + 1]);
                for (int ni = 0; ni < n; ni++) std::swap(mat[ni][i], mat[ni][k + 1]);
            }
            break;
        }
        if (mat[k + 1][k] == 0) continue;
        mint inv = mat[k + 1][k].inv();
        for (int i = k + 2; i < n; i++) {
            mint acc = mat[i][k] * inv;
            for (int j = k; j < n; j++) mat[i][j] -= acc * mat[k + 1][j];
            for (int j = 0; j < n; j++) mat[j][k + 1] += acc * mat[j][i];
        }
    }
    return mat;
}

// Returns the characteristic polynomial of the specified matrix.
// Requires `mat` to be a square matrix.
// Reference: https://judge.yosupo.jp/problem/characteristic_polynomial
template <compatible_modint mint> std::vector<mint> charpoly(const matrix<mint> &mat) {
    if (mat.empty()) return {1};
    int n = mat.size();
    matrix<mint> hess = upper_hessenberg(mat);
    matrix<mint> poly(n + 1);
    poly[0] = {1};
    poly[1] = {-hess[0][0], 1};
    for (int i = 2; i <= n; i++) {
        poly[i].resize(i + 1);
        for (int j = 0; j < i; j++) poly[i][j] = poly[i - 1][j] * -hess[i - 1][i - 1];
        for (int j = 0; j < i; j++) poly[i][j + 1] += poly[i - 1][j];
        mint acc = 1;
        for (int j = 1; j < i; j++) {
            acc *= hess[i - j][i - j - 1];
            mint prod = acc * hess[i - j - 1][i - 1];
            for (int k = 0; k < i - j; k++) poly[i][k] -= prod * poly[i - j - 1][k];
        }
    }
    return poly[n];
}

}  // namespace matrix

}  // namespace kotone

#endif  // KOTONE_MATRIX_HPP
