#pragma once

#include <random>

struct Distribution {

  static long double generate_sigma() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<long double> dist(0.1L, 1.0L);

    return dist(gen);
  }
};
