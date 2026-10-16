#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mathcity {

struct RouteResult {
    std::vector<int> path; // node ids
    std::vector<int> edge_ids;
    double total_cost = 0;
    bool found = false;
};

// traffic: edge_id -> 1,2,3 (default 1). blocked: set of edge ids to skip.
RouteResult dijkstra(int start, int end,
                     const std::unordered_map<int, int>& traffic = {},
                     const std::unordered_set<int>& blocked = {},
                     double a = 1.0, double b = 1.0, double c = 5.0);

RouteResult dijkstra(const std::string& s, const std::string& t,
                      const std::unordered_map<int, int>& traffic = {},
                      const std::unordered_set<int>& blocked = {},
                      double a = 1.0, double b = 1.0, double c = 5.0);

} // namespace mathcity
