// Verified with: https://judge.yosupo.jp/problem/aho_corasick
// Details: https://judge.yosupo.jp/submission/317930

#include <iostream>
#include <vector>
#include <kotone/aho_corasick>

int main() {
    int N;
    std::cin >> N;
    std::vector<std::string> S(N);
    for (std::string &s : S) std::cin >> s;
    kotone::aho_corasick ac;
    for (std::string &s : S) ac.insert(s);
    auto g = ac.to_graph();
    assert(g.characters[0] == char{});
    assert(g.parents[0] == -1);
    assert(g.suffix_links[0] == 0);
    std::cout << g.num_nodes << std::endl;
    for (int i = 1; i < g.num_nodes; i++) {
        std::cout << g.parents[i] << ' ' << g.suffix_links[i] << std::endl;
    }
    for (int i = 0; i < N; i++) std::cout << g.pattern_nodes[i] << ' ';
    std::cout << std::endl;
}
