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
    std::vector<T> _characters;
    std::vector<int> _parents, _suffix_links, _dict_suffix_links;
    std::vector<std::vector<int>> _matches;
    std::vector<Map<T, int>> _children;
    std::vector<int> _pattern_nodes, _pattern_lengths;
    int _last_built_size = 0;

    void _init() {
        _characters.emplace_back();
        _parents.push_back(-1);
        _suffix_links.push_back(0);
        _dict_suffix_links.push_back(-1);
        _matches.emplace_back();
        _children.emplace_back();
    }

    int _num_nodes() const {
        return static_cast<int>(_characters.size());
    }

    int _num_patterns() const {
        return static_cast<int>(_pattern_nodes.size());
    }

    bool _requires_build() const {
        return _last_built_size != _num_nodes();
    }

    void _build(bool full_rebuild = false) {
        std::queue<int> queue;
        if (full_rebuild || _last_built_size == 0) {
            _last_built_size = 1;
            for (auto [ch, child] : _children[0]) {
                _suffix_links[child] = 0;
                queue.push(child);
            }
        } else {
            for (int i = _last_built_size; i < _num_nodes(); i++) queue.push(i);
        }
        while (queue.size()) {
            int curr = queue.front();
            queue.pop();
            for (auto [ch, child] : _children[curr]) {
                int link = _suffix_links[curr];
                while (link > 0 && !_children[link].contains(ch)) {
                    link = _suffix_links[link];
                }
                _suffix_links[child] = _children[link].contains(ch) ? _children[link][ch] : 0;
                queue.push(child);
            }
        }
        for (int i = _last_built_size; i < _num_nodes(); i++) {
            if (_matches[i].size()) _dict_suffix_links[i] = i;
            else {
                int link = _suffix_links[i];
                if (link >= 0) _dict_suffix_links[i] = _dict_suffix_links[link];
            }
        }
        _last_built_size = _num_nodes();
    }

  public:
    // Constructs an empty Aho-Corasick automaton.
    aho_corasick() {
        _init();
    }

    // Inserts the specified pattern into the automaton and returns its index.
    // Requires the pattern to be non-empty.
    template <typename iterable> int insert(const iterable &pattern) {
        static_assert(std::is_convertible_v<typename iterable::value_type, T>);
        int curr = 0;
        int pattern_length = 0;
        for (const T &ch : pattern) {
            pattern_length++;
            if (!_children[curr].contains(ch)) {
                _children[curr][ch] = _num_nodes();
                _characters.push_back(ch);
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
    // If `full_rebuild` is set to `true`, reconstructs suffix links for existing nodes as well.
    void build(bool full_rebuild = false) {
        _build(full_rebuild);
    }

    // Searches the specified string (integer sequence) and returns pairs `(i, j)`
    // for each pattern `j` that matches `string[i]`.
    // The order of pairs is undefined.
    template <typename iterable> std::vector<std::pair<int, int>> search(const iterable &string) {
        static_assert(std::is_convertible_v<typename iterable::value_type, T>);
        if (_requires_build()) _build();
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
    // - `num_nodes` is the number of nodes in the automaton.
    // - node `0` is the root of the automaton.
    // - `pattern_nodes[i]` is the index of the node corresponding to pattern `i`.
    // - `characters[v]` is the character represented by node `v`.
    //   For the root node, `characters[0] == T{}`.
    // - `parents[v]` is the parent of node `v`.
    //   For the root node, `parents[0] == -1`.
    // - `suffix_links[v]` is the suffix link of node `v`.
    //   For the root node, `suffix_links[0] == 0`.
    struct graph {
        int num_nodes;
        std::vector<int> pattern_nodes;
        std::vector<T> characters;
        std::vector<int> parents, suffix_links;
    };

    // Returns the graph representation of the automaton.
    graph to_graph() {
        if (_requires_build()) _build(true);
        return {_num_nodes(), _pattern_nodes, _characters, _parents, _suffix_links};
    }

    // Resets the content of the automaton.
    void clear() {
        _characters.clear();
        _parents.clear();
        _suffix_links.clear();
        _dict_suffix_links.clear();
        _matches.clear();
        _children.clear();
        _pattern_nodes.clear();
        _pattern_lengths.clear();
        _last_built_size = 0;
        _init();
    }
};

}  // namespace kotone

#endif  // KOTONE_AHO_CORASICK_HPP
