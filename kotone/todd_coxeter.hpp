#ifndef KOTONE_TODD_COXETER_HPP
#define KOTONE_TODD_COXETER_HPP 1

#include <vector>
#include <queue>
#include <kotone/dsu>

namespace kotone {

// Returns the right-concatenation transition table of the finite monoid
// defined by the specified string equivalence relation.
// The dimension of the returned table is `L`-by-`size_alphabet`
// where `L` is the number of elements in the monoid.
// State `0` represents the empty string.
// Requires the monoid to be finite.
// Requires `0 <= c < size_alphabet` for every symbol `c` in each string of `relation`.
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/todd_coxeter
std::vector<std::vector<int>> todd_coxeter(
    int size_alphabet,
    const std::vector<std::pair<std::vector<int>, std::vector<int>>> &relation
) {
    std::vector<std::vector<int>> transition;
    kotone::dsu<int> dsu;
    std::vector<bool> done;
    auto add_vertex = [&]() {
        int v = transition.size();
        transition.emplace_back(size_alphabet, -1);
        dsu.add_vertex();
        done.push_back(false);
        return v;
    };
    auto advance = [&](int v, int c) {
        v = dsu.leader(v);
        if (transition[v][c] == -1) transition[v][c] = add_vertex();
        return dsu.leader(transition[v][c]);
    };
    auto walk = [&](int v, const std::vector<int> &s) {
        v = dsu.leader(v);
        for (int c : s) v = advance(v, c);
        return v;
    };
    auto merge = [&](int p, int q) {
        std::queue<std::pair<int, int>> queue;
        queue.emplace(p, q);
        while (queue.size()) {
            auto [u, v] = queue.front();
            queue.pop();
            u = dsu.leader(u);
            v = dsu.leader(v);
            if (u == v) continue;
            int w = dsu.merge(u, v);
            done[w] = done[u] || done[v];
            for (int c = 0; c < size_alphabet; c++) {
                int a = transition[u][c], b = transition[v][c];
                transition[w][c] = a != -1 ? a : b;
                if (a != -1 && b != -1) queue.emplace(a, b);
            }
        }
    };
    add_vertex();
    for (int p = 0; p < (int)transition.size(); p++) {
        if (done[dsu.leader(p)]) continue;
        for (auto [s, t] : relation) merge(walk(p, s), walk(p, t));
        for (int c = 0; c < size_alphabet; c++) advance(p, c);
        done[dsu.leader(p)] = true;
    }
    int V = transition.size();
    std::vector<int> leaders, pos(V, -1);
    leaders.push_back(dsu.leader(0));
    pos[dsu.leader(0)] = 0;
    for (int i = 0; i < V; i++) {
        if (dsu.leader(i) != i || pos[i] != -1) continue;
        pos[i] = leaders.size();
        leaders.push_back(i);
    }
    int L = leaders.size();
    std::vector result(L, std::vector<int>(size_alphabet));
    for (int i = 0; i < L; i++) {
        for (int c = 0; c < size_alphabet; c++) {
            result[i][c] = pos[dsu.leader(transition[leaders[i]][c])];
        }
    }
    return result;
}

}  // namespace kotone

#endif  // KOTONE_TODD_COXETER_HPP
