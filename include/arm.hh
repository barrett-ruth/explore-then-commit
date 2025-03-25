#pragma once

#include <random>

#include "distribution.hh"

struct Arm {
public:
  Arm()
      : total_reward{}, total_pulls{}, sigma(Distribution::generate_sigma()),
        mu(Distribution::generate_mu()) {};

  void pull() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::normal_distribution<long double> dist(mu, sigma);
    total_reward += dist(gen);

    ++total_pulls;
  };

  inline long double calculate_average_reward() {
    return static_cast<long double>(total_reward) / total_pulls;
  }

private:
  long double total_reward;
  long double mu;
  long double sigma;
  std::size_t total_pulls;
};
