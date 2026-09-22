// #include <kotone/misc/matrix.cpp>
// https://github.com/amiast/cpp-utility

#include <vector>
#include <atcoder/modint>

using mint = atcoder::modint998244353;

std::vector<std::vector<mint>> prod(const std::vector<std::vector<mint>> &a, const std::vector<std::vector<mint>> &b) {
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

std::vector<std::vector<mint>> pow(const std::vector<std::vector<mint>> &a, int64_t k) {
    if (k <= 1) return a;
    std::vector<std::vector<mint>> result = pow(a, k / 2);
    result = prod(result, result);
    if (k % 2 == 1) result = prod(result, a);
    return result;
}

// Returns a basis in row echelon form.
std::vector<std::vector<mint>> basis(std::vector<std::vector<mint>> &mat) {
    if (mat.empty()) return {};
    int m = mat[0].size();
    std::vector basis(m, std::vector<mint>(m));
    auto add = [&](std::vector<mint> row) {
        for (int i = 0; i < m; i++) {
            if (row[i] == 0) continue;
            if (basis[i][i] == 0) {
                basis[i] = row;
                return;
            }
            std::swap(basis[i], row);
            mint acc = row[i] / basis[i][i];
            for (int j = 0; j < m; j++) row[j] -= basis[i][j] * acc;
        }
    };
    for (std::vector<mint> &row : mat) add(row);
    return basis;
}

mint determinant(std::vector<std::vector<mint>> mat) {
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

std::pair<std::vector<std::vector<mint>>, bool> inverse(std::vector<std::vector<mint>> mat) {
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
            mint c = mat[i][k];
            for (int j = k; j < n * 2; j++) mat[i][j] -= mat[k][j] * c;
        }
    }
    for (int i = 0; i < n; i++) mat[i].erase(mat[i].begin(), mat[i].begin() + n);
    return {mat, true};
}
