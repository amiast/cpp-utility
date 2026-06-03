// Verified with: https://judge.yosupo.jp/problem/common_interval_decomposition_tree
// Details: https://judge.yosupo.jp/submission/376661

#include <iostream>
#include <vector>
#include <kotone/permutation_interval_tree>

int main() {
    int N;
    std::cin >> N;
    std::vector<int> P(N);
    for (int &p : P) std::cin >> p;

    auto tree = kotone::construct_permutation_interval_tree(P).first;
    int X = tree.size();
    std::cout << X << std::endl;
    for (int i = 0; i < X; i++) {
        std::cout << tree[i].parent << ' ' << tree[i].start << ' ' << tree[i].stop - 1 << ' ';
        if (tree[i].is_linear) std::cout << "linear" << std::endl;
        else std::cout << "prime" << std::endl;
    }
}
