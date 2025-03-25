#pragma once

#include <cmath>
#include <numeric>
#include <vector>

struct Statistics {
  long double get_mean(std::vector<long double> values) {
    if (values.empty())
      return 0.0L;
    return std::accumulate(values.begin(), values.end(), 0.0L) / values.size();
  }

  long double get_standard_deviation(std::vector<long double> values) {
    if (values.size() < 2)
      return 0.0L;
    long double mean = get_mean(values);
    long double variance = 0.0L;
    for (long double value : values) {
      variance += (value - mean) * (value - mean);
    }
    return std::sqrt(variance / values.size());
  }
};
