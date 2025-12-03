#ifndef KOTONE_PERSISTENT_DSU_HPP
#define KOTONE_PERSISTENT_DSU_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>

namespace kotone {

// A partially persistent DSU that monitors connectivity in different versions of a graph.
struct persistent_dsu {
  private:
    int _num_nodes = 0, _version = 0;
    std::vector<std::vector<std::pair<int, int>>> _vec;

  public:
    persistent_dsu() {}

    // Constructs DSU for a graph with the specified number of nodes and no edges.
    // Requires `0 <= num_nodes <= 100000000`.
    persistent_dsu(int num_nodes) : _num_nodes(num_nodes), _vec(num_nodes, {{0, -1}}) {
        assert(0 <= num_nodes && num_nodes <= 100000000);
    }

    // Returns the latest version ID of the graph.
    int latest_version() const {
        return _version;
    }

    // Returns the leader of the connected component containing node `v` in version `t`.
    // If `t == 0`, returns `v`.
    // Requires `0 <= v < num_nodes`.
    // Requires `0 <= t <= latest_version`.
    int leader(int v, int t) const {
        assert(0 <= v && v < _num_nodes);
        assert(0 <= t && t <= _version);
        auto [ver, val] = _vec[v].back();
        if (ver > t || val < 0) return v;
        return leader(val, t);
    }

    // Returns the number of nodes in the connected component containing node `v` in version `t`.
    // If `t == 0`, returns `v`.
    // Requires `0 <= v < num_nodes`.
    // Requires `0 <= t <= latest_version`.
    int size(int v, int t) const {
        assert(0 <= v && v < _num_nodes);
        assert(0 <= t && t <= _version);
        v = leader(v, t);
        return -std::prev(std::upper_bound(_vec[v].begin(), _vec[v].end(), std::pair{t, _num_nodes}))->second;
    }

    // Adds an edge between nodes `u` and `v` then returns a pair containing:
    // - `first`: the leader of the merged component
    // - `second`: the version ID of the new graph
    //
    // Requires `0 <= u < num_nodes`.
    // Requires `0 <= v < num_nodes`.
    std::pair<int, int> add_edge(int u, int v) {
        u = leader(u, _version);
        v = leader(v, _version);
        _version++;
        if (u == v) return {u, _version};
        int size_u = -_vec[u].back().second;
        int size_v = -_vec[v].back().second;
        if (size_u < size_v) std::swap(u, v);
        _vec[u].emplace_back(_version, -size_u - size_v);
        _vec[v].emplace_back(_version, u);
        return {u, _version};
    }
};

}  // namespace kotone

#endif  // KOTONE_PERSISTENT_DSU_HPP
