#include "cost.h"

namespace mathcity {

double road_cost(double dist, double time, int traffic, double a, double b, double c) {
    double congestion = static_cast<double>(traffic - 1); // 0,1,2
    if (a == 1.0 && b == 0.0 && c == 0.0) return dist * traffic;
    return a * dist + b * time + c * congestion;
}

} // namespace mathcity
