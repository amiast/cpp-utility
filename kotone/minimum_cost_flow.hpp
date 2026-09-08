#ifndef KOTONE_MINIMUM_COST_FLOW_HPP
#define KOTONE_MINIMUM_COST_FLOW_HPP 1

#include <vector>
#include <queue>
#include <tuple>
#include <cassert>
#include <atcoder/mincostflow>

namespace kotone {

// A graph representing the minimum-cost network flow problem, in which:
// - each edge has associated cost, upper bound, lower bound; and
// - each node has associated flow balance (demand/supply).
//
// Reference: https://judge.yosupo.jp/problem/min_cost_b_flow
// Reference: https://atcoder.jp/contests/abc393/submissions/62805548
template <typename Cap, typename Cost> struct mincost_network_graph {
    // A minimum-cost network flow.
    // Contains the following attributes:
    // - `feasible`: a `bool` indicating whether the flow exists
    // - `cost`: the cost of the flow
    // - `flow`: a `vector<Cap>` containing the flow of each edge
    // - `potential`: a `vector<Cost>` containing the dual potential of each node
    struct mincost_flow {
        std::vector<Cap> flow;
        std::vector<Cost> potential;
        Cost cost{};
        bool feasible = false;
    };

  private:
    std::vector<std::tuple<int, int, Cap, Cost>> _edges;
    std::vector<Cap> _balance;
    std::vector<bool> _rev;
    mincost_flow _result;

  public:
    mincost_network_graph(int num_nodes) {
        assert(num_nodes >= 0);
        _balance.resize(num_nodes);
    }

    mincost_network_graph() : mincost_network_graph(0) {}

    // Returns the number of nodes in the graph.
    int size() const {
        return _balance.size();
    }

    // Adds a node to the graph and returns its index.
    int add_node() {
        _balance.emplace_back();
        return size() - 1;
    }

    // Adds an edge from `u` to `v` with the specified capacity constraints and cost,
    // then returns the index of the edge.
    // Requires `0 <= u, v < size()`.
    // Requires `lower <= upper`.
    int add_edge(int u, int v, Cap lower, Cap upper, Cost cost) {
        assert(0 <= u && u < size());
        assert(0 <= v && v < size());
        assert(lower <= upper);
        _rev.push_back(cost < Cost{});
        if (_rev.back()) {
            std::swap(u, v);
            std::swap(lower, upper);
            lower = -lower;
            upper = -upper;
            cost = -cost;
        }
        _balance[u] -= lower;
        _balance[v] += lower;
        _result.cost += lower * cost;
        _result.flow.push_back(lower);
        _edges.emplace_back(u, v, upper - lower, cost);
        return _edges.size() - 1;
    }

    // Adds the specified flow balance to node `v`.
    // Requires `0 <= v < size()`.
    void add_balance(int v, Cap balance) {
        assert(0 <= v && v < size());
        _balance[v] += balance;
    }

    // Computes and returns a minimum-cost flow in the network.
    // This method should only be called once.
    mincost_flow flow() {
        atcoder::mcf_graph<Cap, Cost> graph(size() + 2);
        int source = size(), sink = source + 1;
        for (auto &[u, v, cap, cost] : _edges) graph.add_edge(u, v, cap, cost);
        Cap pos{}, neg{};
        for (int v = 0; v < size(); v++) {
            if (_balance[v] == Cap{}) continue;
            if (_balance[v] > Cap{}) {
                graph.add_edge(source, v, _balance[v], Cost{});
                pos += _balance[v];
            } else {
                graph.add_edge(v, sink, -_balance[v], Cost{});
                neg -= _balance[v];
            }
        }
        if (pos != neg) return {};
        auto [f, c] = graph.flow(source, sink);
        if (f < pos) return {};
        _result.feasible = true;
        _result.cost += c;
        std::vector<std::vector<std::pair<int, Cost>>> residual(size());
        for (std::size_t i = 0; i < _result.flow.size(); i++) {
            auto edge = graph.get_edge(i);
            if (edge.flow < edge.cap) residual[edge.from].emplace_back(edge.to, edge.cost);
            if (edge.flow > Cap{}) residual[edge.to].emplace_back(edge.from, -edge.cost);
            _result.flow[i] += edge.flow;
            if (_rev[i]) _result.flow[i] = -_result.flow[i];
        }
        _result.potential.resize(size());
        std::queue<int> queue;
        for (int i = 0; i < size(); i++) queue.push(i);
        std::vector<bool> in_queue(size(), true);
        while (queue.size()) {
            int u = queue.front();
            queue.pop();
            in_queue[u] = false;
            for (auto &[v, c] : residual[u]) {
                if (_result.potential[u] + c >= _result.potential[v]) continue;
                _result.potential[v] = _result.potential[u] + c;
                if (!in_queue[v]) {
                    queue.push(v);
                    in_queue[v] = true;
                }
            }
        }
        return _result;
    }
};

}  // namespace kotone

#endif  // KOTONE_MINIMUM_COST_FLOW_HPP
