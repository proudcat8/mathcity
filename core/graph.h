#pragma once
#include <string>
#include <vector>

namespace mathcity {

struct Node {
    int id;
    std::string name; // R1, N1, H, ...
    std::string kind; // residential, intersection, hospital, fire, school, water, waste
    double x, y;
};

struct Edge {
    int id;
    int u, v;          // node ids
    bool one_way = false; // if true: u -> v only
    double dist = 0;   // Euclidean from coords, filled by graph_load()
    double time = 0;   // dist * 1.0 for now
};

// Locked city: 12 nodes, 14 edges. P1/P2 waste use coords only (perm demo).
// IDs: 0 R1, 1 R2, 2 R3, 3 N1, 4 N2, 5 W, 6 N3, 7 H, 8 F, 9 S, 10 P1, 11 P2
const std::vector<Node>& nodes();
const std::vector<Edge>& edges(); // with dist/time filled

// helpers
int node_id(const std::string& name);
double euclid(double x1, double y1, double x2, double y2);

} // namespace mathcity
