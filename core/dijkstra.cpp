#include "dijkstra.h"
#include "cost.h"
#include "graph.h"
#include <algorithm>
#include <limits>
#include <queue>

namespace mathcity {

RouteResult dijkstra(const std::string& s, const std::string& t,
                     const std::unordered_map<int, int>& traffic,
                     const std::unordered_set<int>& blocked,
                     double a, double b, double c) {
    return dijkstra(node_id(s), node_id(t), traffic, blocked, a, b, c);
}

RouteResult dijkstra(int start, int end,
                     const std::unordered_map<int, int>& traffic,
                     const std::unordered_set<int>& blocked,
                     double a, double b, double c) {
    const auto& ns = nodes();
    const auto& es = edges();
    int n = (int)ns.size();
    const double INF = std::numeric_limits<double>::infinity();

    // adjacency: node -> list of (neighbor, edge_id)
    std::vector<std::vector<std::pair<int,int>>> adj(n);
    for (const auto& e : es) {
        if (blocked.count(e.id)) continue;
        adj[e.u].push_back({e.v, e.id});
        if (!e.one_way) adj[e.v].push_back({e.u, e.id});
    }

    std::vector<double> dist(n, INF);
    std::vector<int> prev_node(n, -1), prev_edge(n, -1);
    using P = std::pair<double,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[start] = 0;
    pq.push({0, start});

    // edge lookup by id
    std::vector<const Edge*> by_id;
    by_id.assign(es.size(), nullptr);
    for (const auto& e : es) by_id[e.id] = &e;

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        if (u == end) break;
        for (auto [v, eid] : adj[u]) {
            const Edge* e = by_id[eid];
            int tr = 1;
            auto it = traffic.find(eid);
            if (it != traffic.end()) tr = it->second;
            double w = road_cost(e->dist, e->time, tr, a, b, c);
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev_node[v] = u;
                prev_edge[v] = eid;
                pq.push({dist[v], v});
            }
        }
    }

    RouteResult r;
    if (dist[end] == INF) return r;
    r.found = true;
    r.total_cost = dist[end];
    for (int cur = end; cur != start; cur = prev_node[cur]) {
        r.path.push_back(cur);
        r.edge_ids.push_back(prev_edge[cur]);
    }
    r.path.push_back(start);
    std::reverse(r.path.begin(), r.path.end());
    std::reverse(r.edge_ids.begin(), r.edge_ids.end());
    return r;
}

} // namespace mathcity
