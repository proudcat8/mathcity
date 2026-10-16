#include "graph.h"
#include <cmath>
#include <stdexcept>

namespace mathcity {

double euclid(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

const std::vector<Node>& nodes() {
    static const std::vector<Node> n = {
        {0,  "R1", "residential",   0.0,  0.0},
        {1,  "R2", "residential",  20.0,  0.0},
        {2,  "R3", "residential",  10.0,  2.0},
        {3,  "N1", "intersection",  5.0,  8.0},
        {4,  "N2", "intersection", 15.0,  8.0},
        {5,  "W",  "water",        10.0,  8.0},
        {6,  "N3", "intersection", 10.0, 14.0},
        {7,  "H",  "hospital",     18.0, 18.0},
        {8,  "F",  "fire",          2.0, 18.0},
        {9,  "S",  "school",       10.0, 20.0},
        {10, "P1", "waste",         2.0,  8.0},
        {11, "P2", "waste",        18.0,  8.0},
    };
    return n;
}

const std::vector<Edge>& edges() {
    static std::vector<Edge> e;
    if (!e.empty()) return e;
    // 14 edges. E4 is one-way N1 -> N2.
    e = {
        {0,  0, 3, false}, // R1-N1
        {1,  1, 4, false}, // R2-N2
        {2,  2, 3, false}, // R3-N1
        {3,  2, 4, false}, // R3-N2
        {4,  3, 4, true},  // N1->N2 one-way
        {5,  3, 5, false}, // N1-W
        {6,  4, 5, false}, // N2-W
        {7,  5, 6, false}, // W-N3
        {8,  3, 6, false}, // N1-N3
        {9,  4, 6, false}, // N2-N3
        {10, 6, 7, false}, // N3-H
        {11, 6, 9, false}, // N3-S
        {12, 3, 8, false}, // N1-F
        {13, 4, 7, false}, // N2-H shortcut
    };
    const auto& n = nodes();
    for (auto& ed : e) {
        const Node& a = n[ed.u];
        const Node& b = n[ed.v];
        ed.dist = euclid(a.x, a.y, b.x, b.y);
        ed.time = ed.dist; // unit speed
    }
    return e;
}

int node_id(const std::string& name) {
    for (const auto& n : nodes())
        if (n.name == name) return n.id;
    throw std::runtime_error("unknown node: " + name);
}

} // namespace mathcity
