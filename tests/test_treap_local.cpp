#include <iostream>
#include <kotone/treap>

using S = std::vector<int>;
S op(S a, S b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}
S e() { return {}; }

int main() {
    // Construction
    std::vector<S> vec(16);
    for (int i = 0; i < 16; i++) vec[i] = {i};
    kotone::treap_manager<S, op, e> treap;
    auto root = treap.allocate_treap(vec);
    assert(treap.size(root) == 16);

    // Reversal
    treap.reverse(root, 3, 12);
    S result = treap.get_prod(root);
    assert(result.size() == 16u);
    for (int i = 0; i < 3; i++) assert(result[i] == i);
    for (int i = 3; i < 12; i++) assert(result[i] + i == 14);
    for (int i = 12; i < 16; i++) assert(result[i] == i);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;

    // Reverse entire treap
    treap.reverse(root);
    std::reverse(result.begin(), result.end());
    assert(treap.get_prod(root) == result);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;

    {
    // Setter/Getter
    for (int i = 0; i < 16; i++) assert(treap.get_val(root, i)[0] == result[i]);
    treap.set_val(root, 7, {99});
    result[7] = 99;
    assert(treap.get_prod(root) == result);
    S subresult = treap.get_prod(root, 5, 14);
    for (int i = 5; i < 14; i++) assert(result[i] == subresult[i - 5]);

    // Split
    auto [r1, r2] = treap.split(root, 5);
    assert(treap.size(r1) == 5);
    assert(treap.size(r2) == 11);
    auto [r3, r4] = treap.split(r2, 9);
    assert(treap.size(r3) == 9);
    assert(treap.size(r4) == 2);
    assert(treap.get_prod(r3) == subresult);

    // Merge
    root = treap.merge(treap.merge(r1, r3), r4);
    assert(treap.size(root) == 16);
    assert(treap.get_prod(root) == result);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Insert single node
    auto new_node = treap.allocate_node({-1});
    treap.insert(root, new_node, 8);
    assert(treap.size(root) == 17);
    assert(treap.get_val(root, 8)[0] == -1);
    result.insert(result.begin() + 8, -1);
    assert(treap.get_prod(root) == result);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Insert treap
    std::vector<S> new_vec{{50}, {51}, {52}};
    auto new_root = treap.allocate_treap(new_vec);
    treap.insert(root, new_root, 4);
    assert(treap.size(root) == 20);
    result.insert(result.begin() + 4, 50);
    result.insert(result.begin() + 5, 51);
    result.insert(result.begin() + 6, 52);
    assert(treap.get_prod(root) == result);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Remove single node
    auto removed_node = treap.remove(root, 13);
    assert(treap.size(root) == 19);
    assert(treap.size(removed_node) == 1);
    assert(treap.get_val(removed_node, 0)[0] == result[13]);
    result.erase(result.begin() + 13);
    assert(treap.get_prod(root) == result);
    treap.deallocate_treap(removed_node);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Remove treap
    auto removed_root = treap.remove(root, 5, 9);
    assert(treap.size(root) == 15);
    assert(treap.size(removed_root) == 4);
    S removed_vec = treap.get_prod(removed_root);
    for (int i = 5; i < 9; i++) assert(result[i] == removed_vec[i - 5]);
    result.erase(result.begin() + 5, result.begin() + 9);
    assert(treap.get_prod(root) == result);
    treap.deallocate_treap(removed_root);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Treap to vector
    std::vector<S> prod_vec = treap.to_vector(root);
    assert(prod_vec.size() == 15u);
    for (int i = 0; i < 15; i++) {
        assert(prod_vec[i].size() == 1u);
        assert(result[i] == prod_vec[i][0]);
    }
    for (S p : prod_vec) std::clog << p[0] << ' ';
    std::clog << std::endl;
    }

    {
    // Binary search: max_right
    auto has_no_negative = [](const S &vec) {
        for (int v : vec) if (v < 0) return false;
        return true;
    };
    int correct_r = 0;
    while (result[correct_r] >= 0) correct_r++;
    assert(correct_r == 7);
    assert(treap.max_right(root, 0, has_no_negative) == correct_r);
    assert(treap.max_right(root, 5, has_no_negative) == correct_r);
    assert(treap.max_right(root, correct_r, has_no_negative) == correct_r);
    assert(treap.max_right(root, 10, has_no_negative) == int(result.size()));
    assert(treap.max_right(root, result.size(), has_no_negative) == int(result.size()));
    treap.set_val(root, 12, {-100});
    result[12] = -100;
    treap.reverse(root, 10, 13);
    std::reverse(result.begin() + 10, result.begin() + 13);
    assert(treap.max_right(root, 5, has_no_negative) == correct_r);
    assert(treap.max_right(root, 8, has_no_negative) == 10);
    assert(treap.max_right(root, 11, has_no_negative) == int(result.size()));
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }

    {
    // Binary search: min_left
    auto has_no_even = [](const S &vec) {
        for (int v : vec) if (v % 2 == 0) return false;
        return true;
    };
    int correct_l1 = 15;
    while (result[correct_l1 - 1] % 2 != 0) correct_l1--;
    assert(correct_l1 == 15);
    int correct_l2 = 10;
    while (result[correct_l2 - 1] % 2 != 0) correct_l2--;
    assert(correct_l2 == 5);
    assert(treap.min_left(root, 15, has_no_even) == correct_l1);
    assert(treap.min_left(root, 10, has_no_even) == correct_l2);
    assert(treap.min_left(root, correct_l2 + 1, has_no_even) == correct_l2);
    assert(treap.min_left(root, 0, has_no_even) == 0);
    treap.reverse(root, 1, 7);
    std::reverse(result.begin() + 1, result.begin() + 7);
    correct_l2 = 10;
    while (result[correct_l2 - 1] % 2 != 0) correct_l2--;
    assert(correct_l2 == 7);
    assert(treap.min_left(root, 10, has_no_even) == correct_l2);
    assert(treap.min_left(root, correct_l2 + 1, has_no_even) == correct_l2);
    for (int r: result) std::clog << r << ' ';
    std::clog << std::endl;
    }
}
