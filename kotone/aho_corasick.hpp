#ifndef KOTONE_AHO_CORASICK_HPP
#define KOTONE_AHO_CORASICK_HPP 1

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <queue>
#include <type_traits>
#include <cassert>

namespace kotone {

template <template <typename...> typename Map>
concept is_allowed_map = (
    std::is_same_v<Map<int, int>, std::map<int, int>>
    || std::is_same_v<Map<int, int>, std::unordered_map<int, int>>
);

// An Aho-Corasick automaton for a set of string patterns.
template <typename T = char, template <typename...> typename Map = std::map>
requires is_allowed_map<Map>
struct aho_corasick {
  private:
    std::vector<int> _parents, _suffix_links, _dict_suffix_links;
    std::vector<std::vector<int>> _matches;
    std::vector<Map<T, int>> _children;
    std::vector<int> _pattern_nodes, _pattern_lengths;
    bool _requires_build = false;

    void _init() {
        _parents.assign(1, -1);
        _suffix_links.assign(1, -1);
        _dict_suffix_links.assign(1, -1);
        _matches.emplace_back();
        _children.emplace_back();
    }

    int _num_nodes() const {
        return static_cast<int>(_parents.size());
    }

    int _num_patterns() const {
        return static_cast<int>(_pattern_nodes.size());
    }

    void _build() {
        _requires_build = false;
        std::queue<int> queue;
        for (auto [ch, child] : _children[0]) {
            _suffix_links[child] = 0;
            queue.push(child);
        }
        while (queue.size()) {
            int curr = queue.front();
            queue.pop();
            for (auto [ch, child] : _children[curr]) {
                int fallback = _suffix_links[curr];
                while (fallback != -1 && !_children[fallback].contains(ch)) {
                    fallback = _suffix_links[fallback];
                }
                _suffix_links[child] = fallback >= 0 && _children[fallback].contains(ch) ? _children[fallback][ch] : 0;
                queue.push(child);
            }
        }
        for (int i = 0; i < _num_nodes(); i++) {
            if (_matches[i].size()) _dict_suffix_links[i] = i;
            else {
                int link = _suffix_links[i];
                _dict_suffix_links[i] = link >= 0 ? _dict_suffix_links[link] : -1;
            }
        }
    }

  public:
    // Constructs an empty Aho-Corasick automaton.
    aho_corasick() {
        _init();
    }

    // Inserts the specified pattern into the automaton and returns its index.
    // Requires the pattern to be non-empty.
    template <typename iterable> int insert(const iterable &pattern) {
        static_assert(std::is_same_v<T, typename iterable::value_type>);
        _requires_build = true;
        int curr = 0;
        int pattern_length = 0;
        for (const T &ch : pattern) {
            pattern_length++;
            if (!_children[curr].contains(ch)) {
                _children[curr][ch] = _num_nodes();
                _parents.push_back(curr);
                _suffix_links.push_back(-1);
                _dict_suffix_links.push_back(-1);
                _matches.emplace_back();
                _children.emplace_back();
            }
            curr = _children[curr][ch];
        }
        assert(pattern_length > 0);
        int pattern_index = _num_patterns();
        _matches[curr].push_back(pattern_index);
        _pattern_nodes.push_back(curr);
        _pattern_lengths.push_back(pattern_length);
        return pattern_index;
    }

    // Forces the automaton to build immediately without lazy evaluation.
    void build() {
        _build();
    }

    // Searches the specified string (integer sequence) and returns pairs `(i, j)`
    // for each pattern `j` that matches `string[i]`.
    // The order of pairs is undefined.
    template <typename iterable> std::vector<std::pair<int, int>> search(const iterable &string) {
        static_assert(std::is_same_v<T, typename iterable::value_type>);
        if (_requires_build) _build();
        std::vector<std::pair<int, int>> result;
        int curr = 0;
        int i = 0;
        for (const T &ch : string) {
            while (curr >= 0 && !_children[curr].contains(ch)) curr = _suffix_links[curr];
            curr = curr >= 0 && _children[curr].contains(ch) ? _children[curr][ch] : 0;
            for (int v = curr; v >= 0; v = _dict_suffix_links[_suffix_links[v]]) {
                for (int j : _matches[v]) result.emplace_back(i - _pattern_lengths[j] + 1, j);
            }
            i++;
        }
        return result;
    }

    // A graph representation of an Aho-Corasick automaton.
    // - `num_nodes` is the number of nodes in the automaton;
    // - node `0` is the root of the automaton;
    // - `pattern_nodes[i]` is the index of the node corresponding to pattern `i`;
    // - `parents[v]` is the parent of node `v`;
    //   (for the root node, `parents[0] == -1`)
    // - `suffix_links[v]` is the suffix link of node `v`.
    struct graph {
        int num_nodes;
        std::vector<int> pattern_nodes, parents, suffix_links;
    };

    // Returns the graph representation of the automaton.
    graph to_graph() {
        if (_requires_build) _build();
        return {_num_nodes(), _pattern_nodes, _parents, _suffix_links};
    }

    // Resets the content of the automaton.
    void clear() {
        _parents.clear();
        _suffix_links.clear();
        _dict_suffix_links.clear();
        _matches.clear();
        _children.clear();
        _pattern_nodes.clear();
        _pattern_lengths.clear();
        _requires_build = false;
        _init();
    }
};

}  // namespace kotone

#endif  // KOTONE_AHO_CORASICK_HPP
