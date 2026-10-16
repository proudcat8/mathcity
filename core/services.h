#pragma once
#include <string>
#include <vector>

namespace mathcity {

struct WasteResult {
    std::vector<int> order; // node ids, starts/ends at depot
    double total = 0;
};

// 4 waste points -> 24 perms, depot defaults to N3 area (W node 5)
std::vector<WasteResult> waste_best(int depot = 5, int top_k = 3);

struct WaterResult {
    double h = 0, s = 0, r = 0; // liters
    double h_pct = 0, s_pct = 0, r_pct = 0;
    double deficit = 0;
};

// ratio h:s:r, e.g. 5:2:3, with minimums H>=h_min etc.
WaterResult water_alloc(double total, double rh, double rs, double rr,
                        double h_min = 400, double s_min = 150, double r_min = 0);

struct ResilienceResult {
    double success_pct = 0;
    double avg_extra_cost = 0;
    int trials = 0;
};

ResilienceResult resilience(const std::string& from, const std::string& to, int trials = 100);

} // namespace mathcity
