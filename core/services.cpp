#include "services.h"
#include "dijkstra.h"
#include "graph.h"
#include <algorithm>
#include <cstdlib>
#include <numeric>

namespace mathcity {

std::vector<WasteResult> waste_best(int depot, int top_k) {
    // Use P1(10), P2(11), R1(0), R2(1) as 4 collection points for 4! = 24 demo.
    // Distances are Euclidean between coords (collection order problem).
    std::vector<int> pts = {10, 11, 0, 1};
    const auto& ns = nodes();
    auto d = [&](int a, int b) {
        return euclid(ns[a].x, ns[a].y, ns[b].x, ns[b].y);
    };
    std::vector<WasteResult> all;
    std::sort(pts.begin(), pts.end());
    do {
        double t = d(depot, pts[0]);
        for (size_t i = 1; i < pts.size(); i++) t += d(pts[i-1], pts[i]);
        t += d(pts.back(), depot);
        WasteResult r;
        r.order = pts;
        r.total = t;
        all.push_back(r);
    } while (std::next_permutation(pts.begin(), pts.end()));
    std::sort(all.begin(), all.end(),
              [](const WasteResult& a, const WasteResult& b){ return a.total < b.total; });
    if ((int)all.size() > top_k) all.resize(top_k);
    return all;
}

WaterResult water_alloc(double total, double rh, double rs, double rr,
                        double h_min, double s_min, double r_min) {
    WaterResult r;
    double sum = rh + rs + rr;
    double h = total * rh / sum, s = total * rs / sum, rr2 = total * rr / sum;
    // enforce minimums by taking from largest share first (simple, explainable)
    if (h < h_min) { double need = h_min - h; h = h_min; rr2 -= need * (rr/(rs+rr)); s -= need * (rs/(rs+rr)); }
    if (s < s_min) { double need = s_min - s; s = s_min; double take = need / 2; h -= take; rr2 -= take; }
    if (rr2 < r_min) { double need = r_min - rr2; rr2 = r_min; h -= need/2; s -= need/2; }
    if (h < 0) h = 0; if (s < 0) s = 0; if (rr2 < 0) rr2 = 0;
    double given = h + s + rr2;
    r.h = h; r.s = s; r.r = rr2;
    r.h_pct = given > 0 ? 100*h/given : 0;
    r.s_pct = given > 0 ? 100*s/given : 0;
    r.r_pct = given > 0 ? 100*rr2/given : 0;
    r.deficit = total < given ? given - total : 0;
    return r;
}

ResilienceResult resilience(const std::string& from, const std::string& to, int trials) {
    RouteResult base = dijkstra(from, to);
    ResilienceResult out;
    out.trials = trials;
    if (!base.found) return out;
    int ok = 0;
    double extra = 0;
    const auto& es = edges();
    for (int i = 0; i < trials; i++) {
        int kill = es[rand() % es.size()].id;
        RouteResult r = dijkstra(from, to, {}, {(int)kill});
        if (r.found) { ok++; extra += (r.total_cost - base.total_cost); }
    }
    out.success_pct = 100.0 * ok / trials;
    out.avg_extra_cost = ok ? extra / ok : 0;
    return out;
}

} // namespace mathcity
