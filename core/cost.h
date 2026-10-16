#pragma once

namespace mathcity {

// Simple: cost = dist * traffic (traffic 1,2,3)
// Advanced: cost = a*D + b*T + c*C
double road_cost(double dist, double time, int traffic, double a = 1.0, double b = 1.0, double c = 5.0);

} // namespace mathcity
